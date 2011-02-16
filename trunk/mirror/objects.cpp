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

Date Inode::get_date(){
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

Inode::Inode(Date d, int s){
    last_change.day=d.day;
    last_change.month=d.month;
    last_change.year=d.year;
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

vector<File>::pointer Directory::get_subfiles(){
    return subfiles;
}

Directory::Directory(string n, Inode* nd){
    name=n;
    node=nd;
}
