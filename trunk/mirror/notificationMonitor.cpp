/*
 *  notificationMonitor.cpp
 *  mirror
 *
 *  Created by Σπύρος Σμπαρούνης on 23/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "notificationMonitor.h"

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
	delete source.root;
	delete target.root;
	source.root = NULL;
	target.root = NULL;
	close(notificationSocket);
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
							   IN_CREATE|IN_DELETE|IN_MOVE|IN_ATTRIB|IN_MODIFY|IN_CLOSE|IN_OPEN);
	
	if (wd < 0) return;
	else assignments.insert(pair<int,directoryElement*>(wd,theElement));
	
	for (delIterator it = theElement->get_contents()->begin(); it != theElement->get_contents()->end(); it++)
		if ((*it)->isDirectory())
			recursiveWatch(*it);
	
	return;
}
