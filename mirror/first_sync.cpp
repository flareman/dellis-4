#include "first_sync.h"


int getdir(string dir, vector<string> &files){
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        perror("Failed to open directory %s :",dir);
        return -1;
    }
    while((dirp = readdir(dp)) != NULL){
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

File_type create_hierarchy(string path,vector<Inode> ndlist){
    struct stat buffer;
    File_type ftp;
    int retval;
    Inode* node;
    retval=stat(path.c_str(),&buffer);
    if(retval!=0){
        perror ("stat() failed on file %s :",path);
        exit(-1);
    }
    else{
        node=new Inode(buffer->st_mtim,buffer->st_size);
        node->set_name(path);
        ndlist.push_back((*node));
        if(S_ISDIR(buffer->st_mode)!=0){    //file is a directory
            Directory* dir;
            dir=new Directory(path,node);
            vector<string> files = vector<string>();
            retval=getdir(path,files);
            if(retval!=0){
                exit(-1);
            }
            for(int i=0;i<files.size();i++){
                File_type ft;
                ft.type=-1;
                ft.obj=NULL;
                ft.nd=NULL;
                ft=create_hierarchy(files[i],ndlist);
                if(ft.type==0){     //files[i] is directory
                    dir->set_subdir(ft.obj);
                }
                else if(ft.type==1){
                    dir->set_subfile(ft.obj);
                }
            }
            ftp.obj=dir;
            ftp.type=0;
            ftp.nd=node;
        }
        else{
            if(S_ISREG(buffer->st_mode)==0){
                cout << "File " << path << "is not a regular file or directory..." << endl;
            }
            File* fl;
            fl=new File(path,node);
            ftp.obj=fl;
            ftp.type=0;
            ftp.nd=node;
        }
    }
    return ftp;
}
