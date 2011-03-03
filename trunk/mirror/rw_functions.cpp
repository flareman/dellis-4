/*
 *  rw_functions.cpp
 *  mirror
 *
 *  Created by Σπύρος Σμπαρούνης on 19/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "rw_functions.h"
#include "notificationMonitor.h"

int copyFile(string initialFilePath, string outputFilePath){
	ifstream initialFile(initialFilePath.c_str(), ios::in|ios::binary);
	ofstream outputFile(outputFilePath.c_str(), ios::out|ios::binary);
	initialFile.seekg(0, ios::end);
	long fileSize = initialFile.tellg();
	if(initialFile.is_open() && outputFile.is_open())	{	
		short* buffer = new short[fileSize/2];	
		initialFile.seekg(0, ios::beg);	
		initialFile.read((char*)buffer, fileSize);	
		outputFile.write((char*)buffer, fileSize);	
		delete[] buffer;
	}
	else if(!outputFile.is_open())	{	
		cout<<"Error opening "<<outputFilePath<<" for writing." << endl;	
		exit(1);
	}
	else if(!initialFile.is_open())	{	
		cout<<"Error opening "<<outputFilePath<<" for reading." << endl;	
		exit(1);
	}		
	initialFile.close();
	outputFile.close();
	return 0;
}

delIterator unlinkElement(directoryElement* theElement, iNodeMap& nodeSet, bool commitRemove) {
	if (theElement->isDirectory())
		for (delIterator it = theElement->get_contents()->begin();it!=theElement->get_contents()->end();it++)
			it = unlinkElement((*it), nodeSet, commitRemove);
	
	if ((commitRemove) && (remove((theElement->getPathToElement()).c_str()))) {
		cerr << "Could not remove " << theElement->get_name() << endl;
		exit(1);
	}

	if (theElement->get_node()->remove_element(theElement) == 0)
                nodeSet.deleteNode(theElement->get_node());
	
	return theElement->get_parent()->remove_element(theElement);
}

void updateAttributes(directoryElement* theElement) {
	struct stat buffer;
	directoryElement* lefty = theElement, *righty = NULL;
	righty = lefty->getCorrespondingElement();
	if (lstat(lefty->getPathToElement().c_str(),&buffer)) {
		cerr << "stat() failed on file " << lefty->get_name() << endl;
		exit(-1);
	}
	lefty->get_node()->set_date(buffer.st_mtime);
	lefty->get_node()->set_size(buffer.st_size);
	chmod(righty->getPathToElement().c_str(), buffer.st_mode & 0000777);
	if (lstat(righty->getPathToElement().c_str(),&buffer)) {
		cerr << "stat() failed on file " << righty->get_name() << endl;
		exit(-1);
	}
	righty->get_node()->set_date(buffer.st_mtime);
	righty->get_node()->set_size(buffer.st_size);
	
	return;
}

void updateFile(directoryElement* theElement) {
	if (theElement == NULL) return;

	if (theElement->isDirectory() == false)
		copyFile(theElement->getPathToElement(), theElement->getCorrespondingElement()->getPathToElement());

	updateAttributes(theElement);
	
	return;
}

directoryElement* createElement(notificationMonitor* theMonitor, directoryElement* theElement, directoryElement* destination, string newName, iNodeMap* nodeMap, bool commitCreate) {
	directoryElement* newElement = NULL;
	Inode* newNode = NULL, *node = NULL;
	struct stat buffer;
	string newPath = destination->getPathToElement()+'/'+newName;
		
	if (commitCreate) {
		if (theElement->isDirectory()) {
			if(lstat((theElement->getPathToElement()).c_str(),&buffer)) {
				cerr << "stat() failed on opening directory " << theElement->getPathToElement() << endl;
				exit(-1);
			}
			mkdir(newPath.c_str(), buffer.st_mode & 0000777);
			if (lstat(newPath.c_str(),&buffer)) {
				cerr << "stat() failed on creating directory " << newPath << endl;
				exit(-1);
			}
			newNode = new Inode(buffer.st_mtime,buffer.st_size,buffer.st_ino);
			node = nodeMap->addNode(newNode);
			delete newNode; newNode = NULL;
			theElement->get_node()->set_target(node);
			newElement = new directoryElement(theElement->get_name(),node,false);
			node->set_element(newElement);
			newElement->set_parent(destination);
                        newElement->set_name(newName);
			for (delIterator it = theElement->get_contents()->begin();it!=theElement->get_contents()->end();it++)
				createElement(theMonitor, (*it), newElement, (*it)->get_name(), nodeMap, true);
		} else {
			if ((node = theElement->get_node()->get_target()) != NULL) {
				newElement = new directoryElement(newName,node,true);
				newElement->set_parent(destination);
                                newElement->set_name(newName);
				link(node->getAnyElement()->getPathToElement().c_str(), newPath.c_str());
                                node->set_element(newElement);
			} else {
				if (lstat((theElement->getPathToElement()).c_str(),&buffer)) {
					cerr << "stat() failed on reading file " << theElement->getPathToElement() << endl;
					exit(-1);
				}
				copyFile(theElement->getPathToElement(), newPath);
				chmod(newPath.c_str(), buffer.st_mode & 0000777);
				if (lstat(newPath.c_str(),&buffer)) {
					cerr << "stat() failed on creating file " << newPath << endl;
					exit(-1);
				}
				newNode = new Inode(buffer.st_mtime,buffer.st_size,buffer.st_ino);
				node = nodeMap->addNode(newNode);
				delete newNode; newNode = NULL;
				theElement->get_node()->set_target(node);
				newElement = new directoryElement(theElement->get_name(),node,true);
				node->set_element(newElement);
				newElement->set_parent(destination);
			}
		}
	} else {
            if (theElement->isDirectory()) {
		if (lstat(newPath.c_str(),&buffer)) {
			cerr << "stat() failed on creating directory " << newPath << endl;
			exit(-1);
		}
		newNode = new Inode(buffer.st_mtime,buffer.st_size,buffer.st_ino);
		node = nodeMap->addNode(newNode);
		delete newNode; newNode = NULL;
		newElement = new directoryElement(newName,node,false);
		node->set_element(newElement);
		newElement->set_parent(destination);
                newElement->set_name(newName);
                newElement->watchDescriptor = theElement->watchDescriptor;
                theMonitor->swapWatch(theElement->watchDescriptor, newElement);
		for (delIterator it = theElement->get_contents()->begin();it!=theElement->get_contents()->end();it++)
			createElement(theMonitor, (*it), newElement, (*it)->get_name(), nodeMap, false);
            } else {
                newElement = new directoryElement(*theElement);
                newElement->set_parent(destination);
                newElement->set_name(newName);
                newElement->get_node()->set_element(newElement);
            }
	}
	
	destination->set_element(newElement);
	
	return newElement;
}
