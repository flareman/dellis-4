/* 
 * File:   first_sync.h
 * Author: dante
 *
 * Created on February 17, 2011, 7:57 PM
 */

#ifndef FIRST_SYNC_H
#define	FIRST_SYNC_H

#include "generalHeaders.h"
#include "rw_functions.h"

using namespace std;

bool compareDirectories (directoryElement first, directoryElement second);
int getdir(string dir, list<string> &files);
directoryElement create_hierarchy(string filename, list<Inode>& nodeList);
directoryElement* recurse_hierarchy(string filename, list<Inode>& ndlist);
void performInitialSync (mirrorEntity source, mirrorEntity target);

#endif	/* FIRST_SYNC_H */

