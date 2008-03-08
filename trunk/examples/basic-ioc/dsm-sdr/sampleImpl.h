/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _sample_impl_h
#define _sample_impl_h

#include <string>
#include <stdio.h>
#include "sdr.h"

class PushPortImpl : public SDR::PushPort
{
	std::string	_name;

  public:
	PushPortImpl(const char* name) : _name(name) {
		printf("[init]: The %s is instantiated\n", name);
	}

	const char* name() { return _name.c_str(); }
};

class LogImpl : public SDR::Log
{
  public:
};

#endif
