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

directoryElement* create_hierarchy(string filename, list<Inode>& ndlist){
    struct stat buffer;
    int retval;
	directoryElement* hierarchyBelow = NULL;
    Inode* node = NULL;
    retval=lstat(filename.c_str(),&buffer);
    if (retval){
        cerr << "stat() failed on file " << filename << "." << endl;
        exit(-1);
    }
    else{
        node=new Inode(buffer.st_mtime,buffer.st_size,buffer.st_ino);
        ndlist.push_back(*node);
        delete node;
        node=&(ndlist.front());
        if(S_ISDIR(buffer.st_mode)!=0){
            directoryElement* dir = NULL;
            dir = new directoryElement(filename,node,false);
			node->set_element(dir);
            list<string> files = list<string>();
            retval=getdir(filename,files);
            if(retval!=0){
                exit(-1);
            }
			for (list<string>::iterator it = files.begin(); it!=files.end(); it++){
				directoryElement* subfolder = NULL;
				subfolder = create_hierarchy(*it,ndlist);
				dir->set_element(subfolder);
				delete subfolder; subfolder = NULL;
            }
            dir->get_contents()->sort(compareDirectories);
			hierarchyBelow = dir;
        } else {
            if(S_ISREG(buffer.st_mode)==0){
                cout << "File " << filename << "is not a regular file or directory..." << endl;
            }
            hierarchyBelow = new directoryElement(filename,node,true);
			node->set_element(hierarchyBelow);
        }
    }
    return hierarchyBelow;
}

void performInitialSync (mirrorEntity source, mirrorEntity target) {
	list<directoryElement>* sourceList = NULL, *targetList = NULL;
	list<directoryElement>::iterator its, itt;
	sourceList = source.parentElement->get_contents();
	targetList = target.parentElement->get_contents();
	its = sourceList->begin(); itt = targetList->begin();

	while (1) {
		if ((its == sourceList->end()) && (itt == targetList->end())) break;
		if (its == sourceList->end()) {itt++; continue;}
		if (itt == targetList->end()) {its++; continue;}
		if ((*itt).get_name() < (*its).get_name()) {
			// Element is reduntant; needs removal
		} else {
			if ((*itt).get_name() == (*its).get_name()) {
				// Same filename; do checks
			} else {
				// Element does not exist in target; replicate
			}

		}
		its++; itt++;
	}
	
	return;
}