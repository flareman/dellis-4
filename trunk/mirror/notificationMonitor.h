/*
 *  notificationMonitor.h
 *  mirror
 *
 *  Created by Σπύρος Σμπαρούνης on 23/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "generalHeaders.h"
#include "rw_functions.h"

using namespace std;

class notificationMonitor {
private:
	map<int,directoryElement*> assignments;
	mirrorEntity source, target;
	int notificationSocket;
	int getdir(string dir, list<string>& files);
	directoryElement* recurse_hierarchy(string filename, string path, iNodeMap& nodeMap);
	void recursiveWatch(directoryElement* theElement);
	void recursiveSync(directoryElement* source, directoryElement* target, iNodeMap& targetNodes);
public:
	void performInitialSync();
	void clearMonitor();
	notificationMonitor() { source.root = NULL; target.root = NULL; };
	~notificationMonitor();
	bool initializeMonitor(char* sourcePath, char* targetPath);
	void assignWatches();
};

bool compareDirectories (directoryElement* first, directoryElement* second);
