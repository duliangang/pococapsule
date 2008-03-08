/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "BumperSensorComponentImpl.h"
#include "sample_c.hh"

BumperSensorComponentImpl::BumperSensorComponentImpl(const char* name) : _name(name)
{ 
}

RTC::ReturnCode_t BumperSensorComponentImpl::connect_SensorDataOut(sample::BumperDataConsumer_ptr consumer)
{
	CORBA::String_var consumer_name = consumer->name();
	printf("[wire]: The %s is connected to the BumperDataOut port of the %s\n", 
			consumer_name.in(), _name.in());
	_consumer = sample::BumperDataConsumer::_duplicate(consumer);

	return 0; // RTC::OK;
}

sample::BumperDataConsumer_ptr BumperSensorComponentImpl::disconnect_SensorDataOut()
{
	return _consumer._retn();
}
