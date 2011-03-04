#ifndef _RW_FUNCTIONS
#define _RW_FUNCTIONS

#include "generalHeaders.h"
#include "notificationMonitor.h"

using namespace std;

delIterator unlinkElement(directoryElement* theElement, iNodeMap& nodeSet, bool commitRemove);
// I'm so sorry
directoryElement* createElement(notificationMonitor* theMonitor, directoryElement* theElement, bool forMove, directoryElement* destination, string newName, iNodeMap* nodeMap, bool commitCreate);
void updateFile(directoryElement* theElement);
void updateAttributes(directoryElement* theElement);
int copyFile(string initialFilePath, string outputFilePath);

#endif
