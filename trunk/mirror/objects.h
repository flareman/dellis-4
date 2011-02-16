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

struct Date{
    int year;
    int month;
    int day;
};

class Inode{
    Date last_change;
    int size;
    vector<string> names;
    int num_of_names;
    Inode* target;
public:
    int get_size();
    Inode* get_target();
    Date get_date();
    vector<string>::pointer get_names();
    int get_num_of_names();
    void set_name(string n);
    Inode(Date d,int s);
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
    vector<Directory> subdir;
    vector<File> subfiles;
public:
    string get_name();
    Inode* get_node();
    vector<Directory>::pointer get_subdir();
    vector<File>::pointer get_subfiles();
    Directory(string n,Inode* nd);
    void set_node(Inode* n);
};

#endif	/* OBJECTS_H */

