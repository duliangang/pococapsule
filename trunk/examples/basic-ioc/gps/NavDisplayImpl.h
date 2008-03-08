/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _NavDisplay_h_
#define _NavDisplay_h_

#include "Interfaces.h"

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

class POCO_EXPORT NavDisplayImpl : public NavDisplay {
	GPSLocator* _locator;
  public:
    NavDisplayImpl(GPSLocator* loc) : _locator(loc) {}
    void refresh();
};

#endif

