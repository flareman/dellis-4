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
	names.push_front(n);
	num_of_names++;
}

directoryElement* Inode::get_an_element() {
	return names.front();
}

int Inode::remove_element(directoryElement* n){
	delIterator it = find(names.begin(), names.end(), n);
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

void Inode::set_target(Inode* newTarget) {
	target = newTarget;
}

void Inode::set_date(time_t newDate) {
	last_change = newDate;
}

void Inode::set_size(int newSize) {
	size = newSize;
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

list<directoryElement*>* directoryElement::get_contents(){
	return &contents;
}

directoryElement* directoryElement::set_element(directoryElement* dir){
	contents.push_front(dir);
	return contents.front();
}

delIterator directoryElement::remove_element(directoryElement* theElement) {
	delIterator it = find(contents.begin(), contents.end(), theElement);
	if (it != contents.end()) {
		delete (*it);
		*it = NULL;
		return contents.erase(it);
	} else return contents.end();
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
	watchDescriptor = -1;
	wasModified = false;
}

directoryElement::~directoryElement() {
	for (delIterator it = contents.begin(); it != contents.end(); it++) {
		delete (*it);
		*it = NULL;
	}
	contents.clear();
}

string directoryElement::getPathToElement() {
	string parentPath;
	if (parent == NULL) parentPath = string("");
	else parentPath = parent->getPathToElement() + '/';
	
	return parentPath + name;
}

void directoryElement::printOutTreeBelow() {
	cout << getPathToElement() << endl;
	if (!isFile)
		for (delIterator it = contents.begin(); it != contents.end(); it++)
		{
			cout << "|";
			(*it)->printOutTreeBelow(2);
		}
	
	return;
}

void directoryElement::printOutTreeBelow(int depth) {
	cout << getPathToElement() << endl;
	if (!isFile)
		for (delIterator it = contents.begin(); it != contents.end(); it++)
		{
			for (int i = 0; i < depth; i++) cout << "|";
			(*it)->printOutTreeBelow(depth+1);
		}
	
	return;
}

directoryElement* directoryElement::getCorrespondingElement() {
	if (node == NULL) return NULL;
	
	if (node->get_target() == NULL) return NULL;
	
	if (node->get_target()->get_num_of_names() < 1) return NULL;
	
	return node->get_target()->get_an_element();
}

directoryElement* directoryElement::elementWithName(string theName) {
	for (delIterator it = contents.begin(); it != contents.end(); it++) {
		if ((*it)->get_name() == theName) return (*it);
	}
	return NULL;
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
