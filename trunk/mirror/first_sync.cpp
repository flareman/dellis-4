#include "first_sync.h"

bool compareDirectories (directoryElement first, directoryElement second) {
	if (first.get_name()<second.get_name()) return true;
	else return false;
}

int getdir(string dir, list<string>& files){
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

directoryElement create_hierarchy(string filename, iNodeMap& nodeMap){
	directoryElement* tmp = NULL;
	tmp = recurse_hierarchy(filename, string(""), nodeMap);
	directoryElement rv = *tmp;
	delete tmp; tmp = NULL;
	return rv;
}

directoryElement* recurse_hierarchy(string filename, string path, iNodeMap& nodeMap){
    struct stat buffer;
    int retval;
	directoryElement* hierarchyBelow = NULL;
    Inode* tmpNode, *node = NULL;
    retval=lstat((path+filename).c_str(),&buffer);
    if (retval){
        cerr << "stat() failed on file " << filename << " (" << retval << ")." << endl;
        exit(-1);
    }
    else{
        tmpNode = new Inode(buffer.st_mtime,buffer.st_size,buffer.st_ino);
        node = nodeMap.addNode(tmpNode);
        delete tmpNode; tmpNode = NULL;
        if(S_ISDIR(buffer.st_mode)){
            directoryElement* dir = NULL;
            dir = new directoryElement(filename,node,false);
			node->set_element(dir);
            list<string> files = list<string>();
            retval=getdir(path+filename,files);
            if(retval!=0){
                exit(-1);
            }
			for (list<string>::iterator it = files.begin(); it!=files.end(); it++){
				directoryElement* subfolder = NULL;
				subfolder = recurse_hierarchy(*it,path+filename+'/',nodeMap);
				subfolder->set_parent(dir);
				dir->set_element(subfolder);
				delete subfolder; subfolder = NULL;
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

void performInitialSync (mirrorEntity source, mirrorEntity target) {
	recursiveSync(source.root, target.root, target.nodes);
	
	return;
}

void recursiveSync (directoryElement* source, directoryElement* target, iNodeMap& targetNodes) {
	list<directoryElement>::iterator its, itt;
	list<directoryElement>* sourceList = NULL, *targetList = NULL;
	sourceList = source->get_contents();
	targetList = target->get_contents();
	its = sourceList->begin(); itt = targetList->begin();
	
	while (1) {
		if ((its == sourceList->end()) && (itt == targetList->end())) break;
		if (its == sourceList->end()) {
			unlinkElement(&(*itt), targetNodes, true);
			itt++; continue;
		}
		if (itt == targetList->end()) {
			createElement(&(*its), target, (*its).get_name(), &targetNodes);
			its++; continue;
		}
		if ((*itt).get_name() < (*its).get_name()) {
			unlinkElement(&(*itt), targetNodes, true);
			continue;
		} else {
			if ((*itt).get_name() == (*its).get_name()) {
				if ((*its).isDirectory() != (*itt).isDirectory()) {
					unlinkElement(&(*itt), targetNodes, true);
					createElement(&(*its), target, (*its).get_name(), &targetNodes);
				} else {
					if ((*its).isDirectory()) {
						recursiveSync(&(*its),&(*itt),targetNodes);
					} else {
						if (((*its).get_node()->get_size() != (*itt).get_node()->get_size()) || ((*its).get_node()->get_date() > (*itt).get_node()->get_date())) {
							unlinkElement(&(*itt), targetNodes, true);
							createElement(&(*its), target, (*its).get_name(), &targetNodes);
						}
					}
				}
			} else {
				createElement(&(*its), target, (*its).get_name(), &targetNodes);
				continue;
			}
		}
		its++; itt++;
	}
}