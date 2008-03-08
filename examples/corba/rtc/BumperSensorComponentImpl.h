/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _bumper_sensor_component_impl_h
#define _bumper_sensor_component_impl_h

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows
#include "sample_c.hh"

class POCO_EXPORT BumperSensorComponentImpl : public sample::BumperSensorComponent
{
	CORBA::String_var	_name;
	sample::BumperDataConsumer_var	_consumer;

  public:
	BumperSensorComponentImpl(const char* name);
	char* name() { return CORBA::string_dup(_name.in()); }
	RTC::ReturnCode_t initialize() { printf("[init]: the %s initialized\n", _name.in()); return 0; }
	RTC::ReturnCode_t finalize() { printf("[fini]: the %s finalized\n", _name.in()); CORBA::release(this); return 0; }

	// emitters
	RTC::ReturnCode_t connect_SensorDataOut(sample::BumperDataConsumer_ptr consumer);
	sample::BumperDataConsumer_ptr disconnect_SensorDataOut();
};

#endif
