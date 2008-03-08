/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
# include "GPSLocatorImpl.h"
# include <stdlib.h>

static int pos_x = 0;
static int pos_y = 200;

int GPSLocatorImpl::get_pos_x()
{
	return pos_x;
}

int GPSLocatorImpl::get_pos_y()
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

void GPSLocatorImpl::subscribe(EventListener* c)
{
	for(int i=0; i<8; i++) {
		if(listeners[i] == NULL) {
			listeners[i] = c;
			break;
		}
	}
}

void GPSLocatorImpl::unsubscribe(EventListener* c)
{
	for(int i=0; i<8; i++) {
		if( listeners[i] == c ) {
			listeners[i] = NULL;
		}
	}
}
