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

int getdir(string dir, vector<string> &files);
File_type create_hierarchy(string path,vector<Inode> ndlist);

#endif	/* FIRST_SYNC_H */

