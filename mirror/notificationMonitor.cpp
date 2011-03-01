/*
 *  notificationMonitor.cpp
 *  mirror
 *
 *  Created by Σπύρος Σμπαρούνης on 23/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "notificationMonitor.h"

bool keepProcessing;

bool notificationMonitor::initializeMonitor(char* sourcePath, char* targetPath) {
	source.root = recurse_hierarchy(string(sourcePath), string(""), source.nodes);
	target.root = recurse_hierarchy(string(targetPath), string(""), target.nodes);
	
	if (source.root && target.root) {
		notificationSocket = inotify_init();
		if (notificationSocket < 0) {
			notificationSocket = 0;
			clearMonitor();
			return false;
		} else return true;
	} else return false;
}

void notificationMonitor::clearMonitor() {
	map<int,directoryElement*>::iterator it;
	for (it = assignments.begin(); it !=assignments.end(); it++)
		inotify_rm_watch(notificationSocket, it->first);
	assignments.clear();
	close(notificationSocket);
	watchedItems = 0;
	delete source.root;
	delete target.root;
	source.root = NULL;
	target.root = NULL;
}

bool compareDirectories (directoryElement* first, directoryElement* second) {
	if (first->get_name()<second->get_name()) return true;
	else return false;
}

int notificationMonitor::getdir(string dir, list<string>& files){
    DIR *dp = NULL;
    struct dirent *dirp = NULL;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cerr << "Failed to open directory %s :" << dir << endl;
        return -1;
    }
    while((dirp = readdir(dp)) != NULL){
		if ((string(dirp->d_name) != ".") && (string(dirp->d_name) != ".."))
			files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

directoryElement* notificationMonitor::recurse_hierarchy(string filename, string path, iNodeMap& nodeMap){
    struct stat buffer;
    int retval;
	directoryElement* hierarchyBelow = NULL;
	Inode* tmpNode = NULL, *node = NULL;
	retval=lstat((path+filename).c_str(),&buffer);
	if (retval){
		cerr << "stat() failed on file " << filename << " (" << retval << ")." << endl;
		return NULL;
	} else {
		tmpNode = new Inode(buffer.st_mtime,buffer.st_size,buffer.st_ino);
		node = nodeMap.addNode(tmpNode);
		delete tmpNode; tmpNode = NULL;
		if(S_ISDIR(buffer.st_mode)){
			directoryElement* dir = NULL;
			dir = new directoryElement(filename,node,false);
			node->set_element(dir);
			list<string> files = list<string>();
			retval=getdir(path+filename,files);
			if(retval != 0){
				return NULL;
			}
			for (list<string>::iterator it = files.begin(); it!=files.end(); it++){
				directoryElement* subfolder = NULL;
				subfolder = recurse_hierarchy(*it,path+filename+'/',nodeMap);
				subfolder->set_parent(dir);
				dir->set_element(subfolder);
			}
			dir->get_contents()->sort(compareDirectories);
			hierarchyBelow = dir;
		} else {
			if(S_ISREG(buffer.st_mode)==0){
				cout << "File " << filename << " is not a regular file or directory..." << endl;
			}
			hierarchyBelow = new directoryElement(filename,node,true);
			node->set_element(hierarchyBelow);
		}
	}
	return hierarchyBelow;
}

void notificationMonitor::performInitialSync () {
	recursiveSync(source.root, target.root, target.nodes);
	
	return;
}

void notificationMonitor::recursiveSync (directoryElement* source, directoryElement* target, iNodeMap& targetNodes) {
	delIterator its, itt;
	list<directoryElement*>* sourceList = NULL, *targetList = NULL;
	sourceList = source->get_contents();
	targetList = target->get_contents();
	its = sourceList->begin(); itt = targetList->begin();
	
	while (1) {
		if ((its == sourceList->end()) && (itt == targetList->end())) break;
		if (its == sourceList->end()) {
			itt = unlinkElement((*itt), targetNodes, true);
			continue;
		}
		if (itt == targetList->end()) {
			createElement((*its), target, (*its)->get_name(), &targetNodes);
			its++; continue;
		}
		if ((*itt)->get_name() < (*its)->get_name()) {
			itt = unlinkElement((*itt), targetNodes, true);
		} else {
			if ((*itt)->get_name() == (*its)->get_name()) {
				if ((*its)->isDirectory() != (*itt)->isDirectory()) {
					itt = unlinkElement((*itt), targetNodes, true);
					createElement((*its), target, (*its)->get_name(), &targetNodes);
					its++;
				} else {
					if ((*its)->isDirectory()) {
						recursiveSync((*its),(*itt),targetNodes);
						its++; itt++;
					} else {
						if (((*its)->get_node()->get_size() != (*itt)->get_node()->get_size()) || ((*its)->get_node()->get_date() > (*itt)->get_node()->get_date())) {
							itt = unlinkElement((*itt), targetNodes, true);
							createElement((*its), target, (*its)->get_name(), &targetNodes);
							its++;
						} else {
							itt++; its++;
						}
					}
				}
			} else {
				createElement((*its), target, (*its)->get_name(), &targetNodes);
				its++;
			}
		}
	}
}

void notificationMonitor::assignWatches() {
	recursiveWatch(source.root);
}

void notificationMonitor::recursiveWatch(directoryElement* theElement) {
	if (theElement == NULL) return;
	int wd = inotify_add_watch(notificationSocket,theElement->getPathToElement().c_str(),
							   IN_CREATE|IN_DELETE|IN_MOVE|IN_MODIFY|IN_DELETE_SELF|IN_ATTRIB|IN_CLOSE_WRITE);
	
	if (wd < 0) return;
	else {
		theElement->watchDescriptor = wd;
		assignments.insert(pair<int,directoryElement*>(wd,theElement));
		watchedItems++;
	}
	
	for (delIterator it = theElement->get_contents()->begin(); it != theElement->get_contents()->end(); it++)
		if ((*it)->isDirectory())
			recursiveWatch(*it);
	
	return;
}

void notificationMonitor::removeWatch(directoryElement* theElement) {
	if (theElement == NULL) return;
	
	assignments.erase(assignments.find(theElement->watchDescriptor));
	inotify_rm_watch(notificationSocket,theElement->watchDescriptor);

	for (delIterator it = theElement->get_contents()->begin(); it != theElement->get_contents()->end(); it++)
		if ((*it)->isDirectory())
			removeWatch(*it);
	
	return;
}

int notificationMonitor::fetchEvents() {
	int theMagicSauce = 0;
	if (currentPosition > 0)
		memcpy(eventBuffer, eventBuffer + currentPosition, eventBufferSize - currentPosition);
	theMagicSauce = read(notificationSocket, eventBuffer + currentPosition, eventBufferSize - currentPosition);
	currentPosition = 0;
	
	return theMagicSauce;
}

bool notificationMonitor::parseEventBuffer() {
	iNotifyEvent *theEvent = NULL;
	while (currentPosition + eventSize <= eventBufferSize) {
		theEvent = (struct inotify_event*) &eventBuffer[currentPosition];
		if (currentPosition + eventSize + theEvent->len > eventBufferSize) break;
		processEvent(theEvent);
		currentPosition += eventSize + theEvent->len;
	}
	if (currentPosition == eventBufferSize) {
		currentPosition = 0;
		return true;
	} else return false;
}

void notificationMonitor::watchForChanges() {
	int bytesRead = 0;
	currentPosition = 0;
	keepProcessing = true;
	if (signal (SIGINT, processSignal) == SIG_IGN) {
		signal (SIGINT, SIG_IGN);
	}
	while (keepProcessing && (watchedItems > 0)) {
		if (checkForEvents()) {
			bytesRead = fetchEvents();
			if (bytesRead < 0) break;
			else parseEventBuffer();
		}
	}
	return;
}

void notificationMonitor::processEvent(iNotifyEvent* theEvent) {
	if (theEvent == NULL) return;
	
	directoryElement* theElement = (assignments.find(theEvent->wd))->second;
	directoryElement* theChild = NULL;
	if (theEvent->len > 0) theChild = theElement->elementWithName(string(theEvent->name));
	
	if (moveCookie != -1) {
		if ((theEvent->mask & IN_MOVED_TO)&&(moveCookie == theEvent->cookie));
		else {
			unlinkElement(moveElement->elementWithName(moveName)->getCorrespondingElement(), target.nodes, true);
			removeWatch(moveElement->elementWithName(moveName));
			unlinkElement(moveElement->elementWithName(moveName), source.nodes, false);
			moveCookie = -1;
			moveName = string("");
			moveElement = NULL;
		}
	}
	
	switch (theEvent->mask & IN_ALL_EVENTS) {
		case IN_MODIFY:
			if ((theChild != NULL)&&(theChild->isDirectory() == false))
				theChild->wasModified = true;
			break;

		case IN_ATTRIB:
			if (theChild != NULL)
				updateAttributes(theChild);
			else updateAttributes(theElement);
			break;
			
		case IN_CLOSE_WRITE:
			if ((theChild != NULL)&&(theChild->isDirectory() == false)&&(theChild->wasModified)) {
				theChild->wasModified = false;
				updateFile(theChild);
			}
			break;
			
		case IN_MOVED_FROM:
			moveCookie = theEvent->cookie;
			moveElement = theElement;
			moveName = string(theEvent->name);
			break;
			
		case IN_MOVED_TO:
			if (moveCookie != -1) {
				moveCookie = -1;
				createElement(moveElement->elementWithName(moveName), theElement, string(theEvent->name), NULL);
				createElement(moveElement->elementWithName(moveName), theElement->getCorrespondingElement(), string(theEvent->name), &target.nodes);
				unlinkElement(moveElement->elementWithName(moveName)->getCorrespondingElement(), target.nodes, true);
				unlinkElement(moveElement->elementWithName(moveName), source.nodes, false);
			} else {
				theChild = recurse_hierarchy(string(theEvent->name), theElement->getPathToElement()+'/', source.nodes);
				createElement(theChild, theElement, theChild->get_name(), NULL);
				createElement(theChild, theElement->getCorrespondingElement(), theChild->get_name(), &target.nodes);
				recursiveWatch(theChild);
			}
			break;
			
		case IN_DELETE:
			if ((theChild != NULL) && (theChild->isDirectory() == false)) {
				unlinkElement(theChild->getCorrespondingElement(), target.nodes, true);
				removeWatch(theChild);
				unlinkElement(theChild, source.nodes, false);
			}
			break;
			
		case IN_DELETE_SELF:
			unlinkElement(theElement->getCorrespondingElement(), target.nodes, true);
			removeWatch(theElement);
			unlinkElement(theElement, source.nodes, false);
			break;
			
		case IN_CREATE:
			theChild = recurse_hierarchy(string(theEvent->name), theElement->getPathToElement()+'/', source.nodes);
			createElement(theChild, theElement, theChild->get_name(), NULL);
			createElement(theChild, theElement->getCorrespondingElement(), theChild->get_name(), &target.nodes);
			recursiveWatch(theChild);
			break;
		default:
			break;
    }
	
	return;
}

int notificationMonitor::checkForEvents() {
	fd_set rfds;
	FD_ZERO (&rfds);
	FD_SET (notificationSocket, &rfds);
	return select (FD_SETSIZE, &rfds, NULL, NULL, NULL);
}

void processSignal (int signum)
{
	keepProcessing = false;
}
