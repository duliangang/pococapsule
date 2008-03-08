/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include <stdio.h>
#include "GreetingImpl.h"

char* MyGreetingNativeImpl::hello(const char* msg)
{
	printf("Server received a %s\n", msg);
	return CORBA::string_dup("hello greeting from Server");
}
