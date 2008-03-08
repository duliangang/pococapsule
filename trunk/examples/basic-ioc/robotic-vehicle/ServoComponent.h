/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _servo_component_impl_h
#define _servo_component_impl_h

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows
#include "objImpls.h"

class POCO_EXPORT ServoComponent 
{
	std::string	_name;
	ServoControl*	_control;
	SpeedDataConsumer*	_consumer;

  public:
	ServoComponent(const char* name);
	const char* name() { return _name.c_str(); }

	// facet
	ServoControl* ControlIn();

	// emitters
	void SpeedDataOut(SpeedDataConsumer* consumer);
};

#endif
