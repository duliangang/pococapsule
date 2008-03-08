/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _servo_component_impl_h
#define _servo_component_impl_h

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows
#include "sample_c.hh"

class POCO_EXPORT ServoComponentImpl : public sample::ServoComponent
{
	CORBA::String_var	_name;
	sample::ServoControl_var	_control;
	sample::SpeedDataConsumer_var	_consumer;

  public:
	ServoComponentImpl(const char* name);
	char* name() { return CORBA::string_dup(_name.in()); }
	RTC::ReturnCode_t initialize() { printf("[init]: the %s initialized\n", _name.in()); return 0; }
	RTC::ReturnCode_t finalize() { printf("[fini]: the %s finalized\n", _name.in()); CORBA::release(this); return 0; }

	// facet
	sample::ServoControl_ptr provide_Control();

	// emitters
	RTC::ReturnCode_t connect_SpeedDataOut(sample::SpeedDataConsumer_ptr consumer);
	sample::SpeedDataConsumer_ptr disconnect_SpeedDataOut();
};

#endif
