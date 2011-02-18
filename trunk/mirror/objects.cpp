#include "objects.h"

//-------------------File Class Implementation---------------------------------------------------//
string File::get_name(){
    return name;
}

Inode* File::get_node(){
    return node;
}

void File::set_node(Inode* nd){
    node=nd;
}

File::File(string n, Inode* nd){
    name=n;
    node=nd;
}

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

void Inode::set_name(string n){
    names.push_back(n);
    num_of_names++ ;
}

vector<string>::pointer Inode::get_names(){
    return names;
}

Inode* Inode::get_target(){
    return target;
}

Inode::Inode(time_t d, int s){
    last_change=d;
    size=s;
    num_of_names=0;
}

//--------------------------------Directory Class Implementation------------------------------------//

string Directory::get_name(){
    return name;
}

Inode* Directory::get_node(){
    return node;
}

void Directory::set_node(Inode* n){
    node=n;
}

vector<Directory>::pointer Directory::get_subdir(){
    return subdir;
}

void Directory::set_subdir(Directory* dir){
    subdir.push_back((*dir));
}

vector<File>::pointer Directory::get_subfiles(){
    return subfiles;
}

void Directory::set_subfile(File* f){
    subfiles.push_back((*f));
}

Directory::Directory(string n, Inode* nd){
    name=n;
    node=nd;
}

Directory::Direcory(const Directory& d){
    d.name=name;
    d.node=node;
    d.subdir=subdir;
    d.subfiles=subfiles;
}
