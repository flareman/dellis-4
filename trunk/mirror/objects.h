/* 
 * File:   objects.h
 * Author: dante
 *
 * Created on January 27, 2011, 7:05 PM
 */

#ifndef OBJECTS_H
#define	OBJECTS_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <sys/stat.h>

using namespace std;

class Inode{
    ino_t serial;
    time_t last_change;
    int size;
    list<string> names;
    int num_of_names;
    Inode* target;
public:
    int get_size();
    Inode* get_target();
    time_t get_date();
    list<string>* get_names();
    int get_num_of_names();
    void set_name(string n);
    Inode(time_t d,int s, ino_t sr);
};

struct File_type{
    int type;
    void* obj;
    Inode* nd;
};

class File{
    string name;
    Inode* node;
public:
    string get_name();
    Inode* get_node();
    void set_node(Inode* nd);
    File(string n,Inode* nd);
};

class Directory{
    string name;
    Inode* node;
    list<Directory> subdir; // sub-directories list
    list<File> subfiles; //sub-files list
public:
    string get_name();
    Inode* get_node();
    list<Directory>* get_subdir();
    void set_subdir(Directory*);
    list<File>* get_subfiles();
    void set_subfile(File*);
    Directory(string n,Inode* nd);
    Directory(const Directory& d);
    void set_node(Inode* n);
};

typedef struct mirrorEntityStruct {
	Directory* theDirectory;
	list<Inode> iNodeList;
} mirrorEntity;

#endif	/* OBJECTS_H */

