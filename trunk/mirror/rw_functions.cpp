/*
 *  rw_functions.cpp
 *  mirror
 *
 *  Created by Σπύρος Σμπαρούνης on 19/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "rw_functions.h"

bool unlinkElement(directoryElement* theElement, iNodeMap& nodeSet) {
	if (theElement->get_parent() == NULL) return false;
	
	if (theElement->isDirectory()) {
		for (list<directoryElement>::iterator it = theElement->get_contents()->begin();it!=theElement->get_contents()->end();it++) {
			unlinkElement(&(*it),nodeSet);
		}
		theElement->get_contents()->clear();
	}
	
	remove((theElement->getPathToElement()).c_str());
	
	if (theElement->get_node()->remove_element(theElement) == 0)
		nodeSet.deleteNode(theElement->get_node());
	
	theElement->get_parent()->remove_element(theElement);
	
	return true;
}