/*
 *  mirror.cpp
 *  mirror
 *
 *  Created by Σπύρος Σμπαρούνης on 19/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "mirror.h"

int main (int argc, char* argv[]) {
	if (argc < 3) {
		cerr << "Error: syntax is mirror <source> <target>" << endl;
		exit(1);
	}
	
	if (!isDirectory(argv[1])) {
		cerr << "Error: specified source \"" << argv[1] << "\" is not a valid directory." << endl;
		exit(1);
	}
	
	if (!isDirectory(argv[2])) {
		cerr << "Error: specified target \"" << argv[1] << "\" is not a valid directory." << endl;
		exit(1);
	}
	
	return 0;
}

bool isDirectory (char* directoryName) {
    struct stat buffer;
    int retval;
    retval = lstat(directoryName, &buffer);
    if (retval){
        cerr << "stat() failed on filename " << directoryName << "." << endl;
        exit(-1);
	}
	return (S_ISDIR(buffer.st_mode));
}
