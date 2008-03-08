/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _GPSLocatorImpl_h_
# define _GPSLocatorImpl_h_

# include "Interfaces_c.hh"
# include <stdlib.h>

# include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

class POCO_EXPORT GPSLocatorImpl : public GPSLocator
{
	EventListener*	listeners[8];

  public:
	GPSLocatorImpl() { for(int i=0; i<8; i++) { listeners[i] = NULL; } }

	CORBA::Long get_pos_x();
	CORBA::Long get_pos_y();

	void refresh();

        void subscribe(EventListener_ptr c);
        void unsubscribe(EventListener_ptr c);
};

#endif
