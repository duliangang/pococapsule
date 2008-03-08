/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "foo.h"
#include <string.h>
#include <stdio.h>

void Foo::okay()
{
	printf("Foo of id %d okay!\n", _id);
}

Foo* Foo::create(int id) 
{
	if( id > 0 ) {
		Foo* foo = new Foo;
		foo->_id = id;
		return foo;
	}

	return NULL;
}

void Foo::report_error(const char* msg)
{
	printf("on-error: %s\n", msg);
}
