/*
 *  rw_functions.cpp
 *  mirror
 *
 *  Created by Σπύρος Σμπαρούνης on 19/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "rw_functions.h"

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

bool unlinkElement(directoryElement* theElement, iNodeMap& nodeSet, bool commitRemove) {
	if (theElement->get_parent() == NULL) return false;
	
	if (theElement->isDirectory())
		for (list<directoryElement>::iterator it = theElement->get_contents()->begin();it!=theElement->get_contents()->end();it++)
			unlinkElement(&(*it), nodeSet, commitRemove);
	
	if ((commitRemove) && (remove((theElement->getPathToElement()).c_str()))) {
		cerr << "Could not remove " << theElement->get_name() << endl;
		exit(1);
	}
	
	if (theElement->get_node()->remove_element(theElement) == 0)
		nodeSet.deleteNode(theElement->get_node());
	
	theElement->get_parent()->remove_element(theElement);
	
	return true;
}

bool createElement(directoryElement* theElement, directoryElement* destination, string newName, iNodeMap* destNodeMap) {
	if (theElement->get_parent() == NULL) return false;
	
	directoryElement* newElement = NULL;
	Inode* newNode = NULL, *node = NULL;
	struct stat buffer;
	string newPath = destination->getPathToElement()+'/'+newName;
		
	if (destNodeMap != NULL) {
		if (theElement->isDirectory()) {
			if(lstat((theElement->getPathToElement()).c_str(),&buffer)) {
				cerr << "stat() failed on file " << theElement->getPathToElement() << endl;
				exit(-1);
			}
			mkdir(newPath.c_str(), buffer.st_mode & 0000777);
			if (lstat(newPath.c_str(),&buffer)) {
				cerr << "stat() failed on file " << newPath << endl;
				exit(-1);
			}
			newNode = new Inode(buffer.st_mtime,buffer.st_size,buffer.st_ino);
			node = destNodeMap->addNode(newNode);
			delete newNode; newNode = NULL;
			theElement->get_node()->set_target(node);
            newElement = new directoryElement(theElement->get_name(),node,false);
			newElement->set_parent(destination);
			for (list<directoryElement>::iterator it = theElement->get_contents()->begin();it!=theElement->get_contents()->end();it++)
				createElement(&(*it), newElement, (*it).get_name(), destNodeMap);
		} else {
			if ((node = theElement->get_node()->get_target()) != NULL) {
				newElement = new directoryElement(newName,node,true);
				newElement->set_parent(destination);
				link(node->get_an_element()->getPathToElement().c_str(), newPath.c_str());
			} else {
				if (lstat((theElement->getPathToElement()).c_str(),&buffer)) {
					cerr << "stat() failed on file " << theElement->getPathToElement() << endl;
					exit(-1);
				}
				copyFile(theElement->getPathToElement(), newPath);
				chmod(newPath.c_str(), buffer.st_mode & 0000777);
				if (lstat(newPath.c_str(),&buffer)) {
					cerr << "stat() failed on file " << newPath << endl;
					exit(-1);
				}
				newNode = new Inode(buffer.st_mtime,buffer.st_size,buffer.st_ino);
				node = destNodeMap->addNode(newNode);
				delete newNode; newNode = NULL;
				theElement->get_node()->set_target(node);
				newElement = new directoryElement(theElement->get_name(),node,true);
				newElement->set_parent(destination);
			}
		}
	} else newElement = theElement;
	
	destination->set_element(newElement);
//	delete newElement; newElement = NULL;
	
	return true;
}
