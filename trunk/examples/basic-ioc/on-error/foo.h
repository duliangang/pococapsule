/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _foo_h_
# define _foo_h_

# include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

class POCO_EXPORT Foo {
	int	_id;

  public:
	void okay();
	static Foo* create(int id);
	static void report_error(const char* msg);
};

#endif
