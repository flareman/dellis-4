/*
 *  rw_functions.h
 *  mirror
 *
 *  Created by Σπύρος Σμπαρούνης on 19/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RW_FUNCTIONS
#define _RW_FUNCTIONS

#include "generalHeaders.h"
#include "notificationMonitor.h"

using namespace std;

delIterator unlinkElement(directoryElement* theElement, iNodeMap& nodeSet, bool commitRemove);
directoryElement* createElement(notificationMonitor* theMonitor, directoryElement* theElement, directoryElement* destination, string newName, iNodeMap* nodeMap, bool commitCreate);
void updateFile(directoryElement* theElement);
void updateAttributes(directoryElement* theElement);
int copyFile(string initialFilePath, string outputFilePath);

#endif