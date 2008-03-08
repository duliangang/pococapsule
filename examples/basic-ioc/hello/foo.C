/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "foo.h"
#include <string.h>
#include <stdio.h>

Foo::Foo(const char* msg) 
{
	printf("Hello %s\n", msg);
}

Foo::~Foo()
{
	printf("That's all folks! Goodbye!\n");
}

void Foo::hello(const char* msg)
{
	printf("Hello %s\n", msg);
}
