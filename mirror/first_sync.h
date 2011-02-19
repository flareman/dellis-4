/* 
 * File:   first_sync.h
 * Author: dante
 *
 * Created on February 17, 2011, 7:57 PM
 */

#ifndef FIRST_SYNC_H
#define	FIRST_SYNC_H

#define _POSIX_C_SOURCE 199309L

#include "objects.h"
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

bool compareDirectories (directoryElement first, directoryElement second);
int getdir(string dir, list<string> &files);
directoryElement* create_hierarchy(string filename,list<Inode>& ndlist);
void performInitialSync (mirrorEntity source, mirrorEntity target);

#endif	/* FIRST_SYNC_H */
