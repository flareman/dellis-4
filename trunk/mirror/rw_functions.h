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

using namespace std;

bool unlinkElement(directoryElement* theElement, iNodeMap& nodeSet, bool commitRemove);
bool createElement(directoryElement* theElement, directoryElement* destination, string newName, iNodeMap* destNodeMap);
int copyFile(string initialFilePath, string outputFilePath);

#endif