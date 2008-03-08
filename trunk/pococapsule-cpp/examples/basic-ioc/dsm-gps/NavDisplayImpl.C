/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "NavDisplayImpl.h"
#include <stdio.h>

void NavDisplayImpl::refresh()
{
	printf("current location is {%3d, %3d}\n", 
		_locator->get_pos_x(),
		_locator->get_pos_y());
}
