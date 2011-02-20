#include "objects.h"

//-----------------------------Inode Class Implementation-----------------------------------------//

time_t Inode::get_date(){
    return last_change;
}

int Inode::get_num_of_names(){
    return num_of_names;
}

int Inode::get_size(){
    return size;
}

ino_t Inode::get_serial(){
    return serial;
}

void Inode::set_element(directoryElement* n){
    names.push_back(n);
    num_of_names++;
}

int Inode::remove_element(directoryElement* n){
	list<directoryElement*>::iterator it = find(names.begin(), names.end(), n);
	if (it != names.end()) {
		num_of_names--;
		names.erase(it);
	}
    return num_of_names;
}

list<directoryElement*>* Inode::get_names(){
    return &names;
}

Inode* Inode::get_target(){
    return target;
}

Inode::Inode(time_t d, int s, ino_t sr){
    last_change=d;
    size=s;
    num_of_names=0;
    serial=sr;
	target = NULL;
}

//--------------------------------Directory Class Implementation------------------------------------//

string directoryElement::get_name(){
    return name;
}

bool directoryElement::isDirectory(){
    return (!isFile);
}

Inode* directoryElement::get_node(){
    return node;
}

void directoryElement::set_node(Inode* n){
    node=n;
}

void directoryElement::set_parent(directoryElement* theParent){
	parent = theParent;
	return;
}

directoryElement* directoryElement::get_parent() {
	return parent;
}

list<directoryElement>* directoryElement::get_contents(){
    return &contents;
}

void directoryElement::set_element(directoryElement* dir){
    contents.push_back(*dir);
}

void directoryElement::remove_element(directoryElement* theElement) {
	list<directoryElement>::iterator it = find(contents.begin(), contents.end(), *theElement);
	if (it != contents.end()) {
		contents.erase(it);
	}
    return;	
}

bool directoryElement::operator==(const directoryElement &x) const {
    if (name != x.name) return false;
    if (node != x.node) return false;
    if (isFile != x.isFile) return false;
	if (parent != x.parent) return false;
	
    return (contents == x.contents);
}

bool directoryElement::operator!=(const directoryElement &x) const {
    return (!((*this) == x));
}

directoryElement::directoryElement(string n, Inode* nd, bool isNewFile){
    name=n;
    node=nd;
	isFile = isNewFile;
	parent = NULL;
}

string directoryElement::getPathToElement() {
	string parentPath;
	if (parent == NULL) parentPath = string("");
	else parentPath = parent->getPathToElement() + '/';
	
	return parentPath + name;
}

void directoryElement::printOutTreeBelow() {
	cout << name << endl;
	if (!isFile)
		for (list<directoryElement>::iterator it = contents.begin(); it != contents.end(); it++)
		{
			cout << "|";
			(*it).printOutTreeBelow(2);
		}
	
	return;
}

void directoryElement::printOutTreeBelow(int depth) {
	cout << name << endl;
	if (!isFile)
		for (list<directoryElement>::iterator it = contents.begin(); it != contents.end(); it++)
		{
			for (int i = 0; i < depth; i++) cout << "|";
			(*it).printOutTreeBelow(depth+1);
		}
	
	return;
}

Inode* iNodeMap::nodeWithID(ino_t theID) {
	map<ino_t, Inode>::iterator it;
	it = nodes.find(theID);
	if (it == nodes.end()) return NULL;
	return &(it->second);
}

void iNodeMap::deleteNode(Inode* theNode) {
	nodes.erase(nodes.find(theNode->get_serial()));
	return;
}

Inode* iNodeMap::addNode(Inode* newNode) {
	pair<map<ino_t, Inode>::iterator,bool> res = nodes.insert(pair<ino_t,Inode>(newNode->get_serial(),*newNode));	
	return &(res.first->second);
}
