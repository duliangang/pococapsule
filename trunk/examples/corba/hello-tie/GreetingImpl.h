/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _greeting_impl_h_
# define _greeting_impl_h_

# include "Greeting_s.hh"
# include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

//
// Server's bean implemenation, not inherited from POA skeleton!
//
class POCO_EXPORT MyGreetingNativeImpl 
{
   public:
	char* hello(const char* s);
};

#endif
