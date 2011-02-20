/*
 *  rw_functions.cpp
 *  mirror
 *
 *  Created by Σπύρος Σμπαρούνης on 19/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "rw_functions.h"

bool unlinkElement(directoryElement* theElement, iNodeMap& nodeSet, bool commitRemove) {
	if (theElement->get_parent() == NULL) return false;
	
	if (theElement->isDirectory())
		for (list<directoryElement>::iterator it = theElement->get_contents()->begin();it!=theElement->get_contents()->end();it++)
			unlinkElement(&(*it),nodeSet, commitRemove);
	
	if ((commitRemove) && (remove((theElement->getPathToElement()).c_str()))) {
		cerr << "Could not remove " << theElement->get_name() << endl;
		exit(1);
	}
	
	if (theElement->get_node()->remove_element(theElement) == 0)
		nodeSet.deleteNode(theElement->get_node());
	
	theElement->get_parent()->remove_element(theElement);
	
	return true;
}