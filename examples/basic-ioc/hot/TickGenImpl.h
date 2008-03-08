/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _TickGen_h_
# define _TickGen_h_

# include "Interfaces.h"
# include <stdlib.h>

# include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

class POCO_EXPORT TickGenImpl : public TickGen 
{
	int	count;
	int	pulse_interval;
	EventListener*	listeners[8];
	
  public:
	TickGenImpl(int cnt, int interval) 
		: count(cnt), pulse_interval(interval) {
        	for(int i=0; i<8; i++) { listeners[i] = NULL; } }
	void start();
	void subscribe(EventListener* c);
	void unsubscribe(EventListener* c);
};

#endif
