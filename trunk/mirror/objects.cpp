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

void Inode::set_element(directoryElement* n){
    names.push_back(n);
    num_of_names++ ;
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
}

//--------------------------------Directory Class Implementation------------------------------------//

string directoryElement::get_name(){
    return name;
}

Inode* directoryElement::get_node(){
    return node;
}

void directoryElement::set_node(Inode* n){
    node=n;
}

list<directoryElement>* directoryElement::get_contents(){
    return &contents;
}

void directoryElement::set_element(directoryElement* dir){
    contents.push_back(*dir);
}

directoryElement::directoryElement(string n, Inode* nd, bool isNewFile){
    name=n;
    node=nd;
	isFile = isNewFile;
}

directoryElement::directoryElement(const directoryElement& d){
    name = d.name;
    node = d.node;
    contents = d.contents;
	isFile = d.isFile;
}
