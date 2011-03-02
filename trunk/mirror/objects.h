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
typedef list<directoryElement*>::iterator delIterator;
typedef list<directoryElement*>::const_iterator delConstIterator;

class Inode{
private:
	ino_t serial;
	time_t last_change;
	int size;
	int num_of_names;
	Inode* target;
	list<directoryElement*> names;
public:
        int get_size();
	Inode* get_target();
	void set_target(Inode* newTarget);
	time_t get_date();
	ino_t get_serial();
	void set_date(time_t newDate);
	void set_size(int newSize);
	list<directoryElement*> *get_names();
	int get_num_of_names();
	void set_element(directoryElement* n);
	directoryElement* getCounterpart(string relativePath);
        directoryElement* getAnyElement();
	int remove_element(directoryElement* n);
	Inode(time_t d,int s, ino_t sr);
};

class directoryElement{
private:
	string name;
	Inode* node;
	bool isFile;
	list<directoryElement*> contents;
	directoryElement* parent;
public:
	string get_name();
	void set_name(string newName);
	Inode* get_node();
	directoryElement* get_parent();
	bool isDirectory();
	void set_parent(directoryElement* theParent);
	list<directoryElement*>* get_contents();
	directoryElement* set_element(directoryElement* newElement);
	delIterator remove_element(directoryElement* theElement);
	directoryElement(string n, Inode* nd, bool isNewFile);
        directoryElement(const directoryElement &theElement);
	~directoryElement();
	bool operator==(const directoryElement &x) const;
	bool operator!=(const directoryElement &x) const;
	void set_node(Inode* n);
	string getPathToElement();
	string getRelativePathToElement();
	void printOutTreeBelow();
	void printOutTreeBelow(int depth);
	directoryElement* getCorrespondingElement();
	directoryElement* elementWithName(string theName);
	int watchDescriptor;
	bool wasModified;
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
