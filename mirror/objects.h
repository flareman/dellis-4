/* 
 * File:   objects.h
 * Author: dante
 *
 * Created on January 27, 2011, 7:05 PM
 */

#ifndef OBJECTS_H
#define	OBJECTS_H

#include "generalHeaders.h"

using namespace std;

class directoryElement;

class Inode{
    ino_t serial;
    time_t last_change;
    int size;
    list<directoryElement*> names;
    int num_of_names;
    Inode* target;
public:
    int get_size();
    Inode* get_target();
    time_t get_date();
    list<directoryElement*>* get_names();
    int get_num_of_names();
    void set_element(directoryElement* n);
    Inode(time_t d,int s, ino_t sr);
};

class directoryElement{
    string name;
    Inode* node;
    bool isFile;
    list<directoryElement> contents;
public:
    string get_name();
    Inode* get_node();
    list<directoryElement>* get_contents();
    void set_element(directoryElement*);
	directoryElement(string n, Inode* nd, bool isNewFile);
    directoryElement(const directoryElement& d);
    void set_node(Inode* n);
};

typedef struct mirrorEntityStruct {
	directoryElement* parentElement;
	list<Inode> iNodeList;
} mirrorEntity;

#endif	/* OBJECTS_H */

