/*
 *  notificationMonitor.h
 *  mirror
 *
 *  Created by Σπύρος Σμπαρούνης on 23/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NOTIFICATION_MONITOR_H
#define _NOTIFICATION_MONITOR_H

#include "generalHeaders.h"
#include "rw_functions.h"
#include <signal.h>

typedef struct inotify_event iNotifyEvent;

const int eventSize = sizeof(iNotifyEvent);
const int eventBufferSize = 1024*(eventSize+32);

using namespace std;

class notificationMonitor {
private:
	map<int,directoryElement*> assignments;
	mirrorEntity source, target;
	int notificationSocket;
	char eventBuffer[eventBufferSize];
	int currentPosition;
	int watchedItems;

	string moveName;
	int moveCookie;
	directoryElement* moveElement;
	
	int getdir(string dir, list<string>& files);
	directoryElement* recurse_hierarchy(string filename, string path, iNodeMap& nodeMap);
	void recursiveWatch(directoryElement* theElement);
	void removeWatch(directoryElement* theElement);
	void recursiveSync(directoryElement* source, directoryElement* target, iNodeMap& targetNodes);
	int fetchEvents();
	bool parseEventBuffer();
	void processEvent(iNotifyEvent* theEvent);
	int checkForEvents();
public:
	void performInitialSync();
	void clearMonitor();
	notificationMonitor() {
		source.root = NULL; target.root = NULL;
		watchedItems = 0;
		memset(&eventBuffer, 0, eventBufferSize); currentPosition = 0;
		moveCookie = -1;
		moveElement = NULL;
		moveName = string("");
	};
	~notificationMonitor() { clearMonitor(); };
	bool initializeMonitor(char* sourcePath, char* targetPath);
	void assignWatches();
	void watchForChanges();
};

bool compareDirectories (directoryElement* first, directoryElement* second);
void processSignal(int signum);

#endif
