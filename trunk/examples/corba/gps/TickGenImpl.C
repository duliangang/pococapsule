/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
# include "TickGenImpl.h"
# include <stdlib.h>
# include <stdio.h>
# ifndef WIN32
#   include <unistd.h>
#   define SLEEP(t) sleep(t)
# else
#   include <windows.h>
#   define SLEEP(t) Sleep(t)
# endif

void TickGenImpl::start()
{
	printf("generate %d pulses in %d seconds\n", 
			count, count*pulse_interval);

	for(int i=0;i<count;i++) {
		for(int i=0; i<8; i++) {
			if( listeners[i] != NULL ) {
				listeners[i]->refresh();
			}
		}

		SLEEP(pulse_interval);
	}
}

void TickGenImpl::subscribe(EventListener_ptr c)
{
	for(int i=0; i<8; i++) {
		if(listeners[i] == NULL) {
			listeners[i] = c;
			break;
		}
	}
}

void TickGenImpl::unsubscribe(EventListener* c)
{
	for(int i=0; i<8; i++) {
		if( listeners[i] == c ) {
			listeners[i] = NULL;
		}
	}
}
