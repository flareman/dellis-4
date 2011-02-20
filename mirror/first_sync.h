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
directoryElement create_hierarchy(string filename, iNodeMap& nodeMap);
directoryElement* recurse_hierarchy(string filename, string path, iNodeMap& nodeMap);
void performInitialSync (mirrorEntity source, mirrorEntity target);
void recursiveSync (directoryElement* source, directoryElement* target, iNodeMap& targetNodes);

#endif	/* FIRST_SYNC_H */

