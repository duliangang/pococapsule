/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _NavDisplay_h_
#define _NavDisplay_h_

#include "Interfaces_c.hh"

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

class POCO_EXPORT NavDisplayImpl : public NavDisplay {
	GPSLocator_ptr _locator;
  public:
    NavDisplayImpl(GPSLocator_ptr loc) : _locator(loc) {}
    void refresh();
};

#endif

