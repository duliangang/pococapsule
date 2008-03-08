/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _foo_h_
# define _foo_h_

# include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

class POCO_EXPORT Foo {
  public:
	Foo(const char* msg);
	~Foo();
	void hello(const char* msg);
};

#endif
