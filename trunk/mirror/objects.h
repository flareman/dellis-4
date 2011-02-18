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
#include <vector>

using namespace std;

struct File_type{
    int type;
    void* obj;
    Inode* nd;
};

class Inode{
    time_t last_change;
    int size;
    vector<string> names;
    int num_of_names;
    Inode* target;
public:
    int get_size();
    Inode* get_target();
    time_t get_date();
    vector<string>::pointer get_names();
    int get_num_of_names();
    void set_name(string n);
    Inode(time_t d,int s);
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
    vector<Directory> subdir; // sub-directories list
    vector<File> subfiles; //sub-files list
public:
    string get_name();
    Inode* get_node();
    vector<Directory>::pointer get_subdir();
    void set_subdir(Directory*);
    vector<File>::pointer get_subfiles();
    void set_subfile(File*);
    Directory(string n,Inode* nd);
    Direcory(const Directory& d);
    void set_node(Inode* n);
};

#endif	/* OBJECTS_H */

