/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _months_reader_h
# define _months_reader_h
#include <map>
#include <string>
#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

POCO_EXPORT void read_props(std::map<std::string, std::string>& props);

#endif
