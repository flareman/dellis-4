#include "first_sync.h"

using namespace std;

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
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

File_type create_hierarchy(string filename, string path, list<Inode*>& ndlist){
    struct stat buffer;
    File_type ftp;
    int retval;
    Inode* node = NULL;
    retval=lstat(filename.c_str(),&buffer);
    if(retval!=0){
        cerr << "stat() failed on file %s :" << filename << endl;
        exit(-1);
    }
    else{
        node=new Inode(buffer.st_mtime,buffer.st_size,buffer.st_ino);
        node->set_name(path+filename);
        ndlist.push_back(node);
        if(S_ISDIR(buffer.st_mode)!=0){    //file is a directory
            directoryElement* dir = NULL;
            dir=new directoryElement(filename,node,false);
            list<string> files = list<string>();
            retval=getdir(filename,files);
            if(retval!=0){
                exit(-1);
            }
	    for(list<string>::iterator it = files.begin(); it!=files.end(); it++){
                File_type ft;
                ft.type=-1;
                ft.obj=NULL;
                ft.nd=NULL;
                ft=create_hierarchy(*it,path+'/'+filename+'/',ndlist);
		dir->set_element(ft.obj);
                delete ft.obj;
                ft.obj=NULL;
            }
            dir->get_contents()->sort(compareDirectories);
            ftp.obj=dir;
            ftp.type=0;
            ftp.nd=node;
        }
        else{
            if(S_ISREG(buffer.st_mode)==0){
                cout << "File " << filename << "is not a regular file or directory..." << endl;
            }
            directoryElement* fl = NULL;
            fl=new directoryElement(filename,node,true);
            ftp.obj=fl;
            ftp.type=1;
            ftp.nd=node;
        }
    }
    return ftp;
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