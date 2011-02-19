#include "first_sync.h"

using namespace std;

bool compareDirectories (directoryElement first, directoryElement second) {
	if (first.get_name()<second.get_name()) return true;
	else return false;
}

int getdir(string dir, list<string> &files){
    DIR *dp;
    struct dirent *dirp;
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

File_type create_hierarchy(string path,list<Inode> &ndlist){
    struct stat buffer;
    File_type ftp;
    int retval;
    Inode* node;
    retval=lstat(path.c_str(),&buffer);
    if(retval!=0){
        cerr << "stat() failed on file %s :" << path << endl;
        exit(-1);
    }
    else{
        node=new Inode(buffer.st_mtime,buffer.st_size,buffer.st_ino);
        node->set_name(path);
        ndlist.push_back((*node));
        if(S_ISDIR(buffer.st_mode)!=0){    //file is a directory
            directoryElement* dir;
            dir=new directoryElement(path,node,false);
            list<string> files = list<string>();
            retval=getdir(path,files);
            if(retval!=0){
                exit(-1);
            }
			for(list<string>::iterator it = files.begin(); it!=files.end(); it++){
                File_type ft;
                ft.type=-1;
                ft.obj=NULL;
                ft.nd=NULL;
                ft=create_hierarchy(*it,ndlist);
				dir->set_element((directoryElement*)ft.obj);
            }
			dir->get_contents()->sort(compareDirectories);
            ftp.obj=(directoryElement*)dir;
            ftp.type=0;
            ftp.nd=node;
        }
        else{
            if(S_ISREG(buffer.st_mode)==0){
                cout << "File " << path << "is not a regular file or directory..." << endl;
            }
            directoryElement* fl;
            fl=new directoryElement(path,node,true);
            ftp.obj=(directoryElement*)fl;
            ftp.type=1;
            ftp.nd=node;
        }
    }
    return ftp;
}

void performInitialSync (mirrorEntity source, mirrorEntity target) {
	// Perform sync here
	
	return;
}