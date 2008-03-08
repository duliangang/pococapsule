/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _GPSLocatorImpl_h_
# define _GPSLocatorImpl_h_

# include "Interfaces.h"
# include <stdlib.h>

# include <pocodefs.h>

class POCO_EXPORT GPSLocatorImpl : public GPSLocator
{
	EventListener*	listeners[8];

  public:
	GPSLocatorImpl() { for(int i=0; i<8; i++) { listeners[i] = NULL; } }

	int get_pos_x();
	int get_pos_y();

	void refresh();

        void subscribe(EventListener* c);
        void unsubscribe(EventListener* c);
};

#endif
