/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
# include "GPSLocatorImpl.h"
# include <stdlib.h>

static CORBA::Long pos_x = 0;
static CORBA::Long pos_y = 200;

CORBA::Long GPSLocatorImpl::get_pos_x()
{
	return pos_x;
}

CORBA::Long GPSLocatorImpl::get_pos_y()
{
	return pos_y;
}

void GPSLocatorImpl::refresh()
{
	pos_x++;
	pos_y--;

	for(int i=0; i<8; i++) {
		if( listeners[i] != NULL ) {
			listeners[i]->refresh();
		}
	}
}

void GPSLocatorImpl::subscribe(EventListener_ptr c)
{
	for(int i=0; i<8; i++) {
		if(listeners[i] == NULL) {
			listeners[i] = c;
			break;
		}
	}
}

void GPSLocatorImpl::unsubscribe(EventListener_ptr c)
{
	for(int i=0; i<8; i++) {
		if( listeners[i] == c ) {
			listeners[i] = NULL;
		}
	}
}
