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
private:
    ino_t serial;
    time_t last_change;
    int size;
    list<directoryElement*> names;
    int num_of_names;
    Inode* target;
public:
    int get_size();
    Inode* get_target();
    void set_target(Inode* newTarget);
    time_t get_date();
	ino_t get_serial();
    list<directoryElement*> *get_names();
    int get_num_of_names();
    void set_element(directoryElement* n);
	directoryElement* get_an_element();
	int remove_element(directoryElement* n);
    Inode(time_t d,int s, ino_t sr);
};

class directoryElement{
private:
    string name;
    Inode* node;
    bool isFile;
    list<directoryElement> contents;
	directoryElement* parent;
public:
    string get_name();
    Inode* get_node();
    directoryElement* get_parent();
	bool isDirectory();
	void set_parent(directoryElement* theParent);
    list<directoryElement>* get_contents();
    void set_element(directoryElement* newElement);
	void remove_element(directoryElement* theElement);
	directoryElement(string n, Inode* nd, bool isNewFile);
	~directoryElement() {};
	bool operator==(const directoryElement &x) const;
	bool operator!=(const directoryElement &x) const;
    void set_node(Inode* n);
	string getPathToElement();
	void printOutTreeBelow();
	void printOutTreeBelow(int depth);
};

class iNodeMap {
private:
	map<ino_t,Inode> nodes;
public:
	iNodeMap() {};
	~iNodeMap() { nodes.clear(); };
	Inode* nodeWithID(ino_t theID);
	void deleteNode(Inode* theNode);
	Inode* addNode(Inode* newNode);
};

typedef struct mirrorEntityStruct {
	directoryElement* root;
	iNodeMap nodes;
} mirrorEntity;

#endif	/* OBJECTS_H */

