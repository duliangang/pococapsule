/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "ServoComponentImpl.h"

class ServoControlImpl : public sample::ServoControl 
{
	CORBA::String_var	_name;
  public:
	ServoControlImpl(const char* name) : _name(name) {}
	char*	name() { return CORBA::string_dup(_name.in()); }
};

ServoComponentImpl::ServoComponentImpl(const char* name) : _name(name)
{
	char buf[256];
	sprintf(buf, "Control facet of the %s", name);
	_control = new ServoControlImpl(buf); 
}

sample::ServoControl_ptr ServoComponentImpl::provide_Control()
{
	return sample::ServoControl::_duplicate(_control.in());
}

RTC::ReturnCode_t ServoComponentImpl::connect_SpeedDataOut(sample::SpeedDataConsumer_ptr consumer)
{
	CORBA::String_var consumer_name = consumer->name();
	printf("[wire]: The %s is connected to the SpeedDataOut port of %s\n", 
			consumer_name.in(), _name.in());
	_consumer = sample::SpeedDataConsumer::_duplicate(consumer);

	return 0; // RTC::OK;
}

sample::SpeedDataConsumer_ptr ServoComponentImpl::disconnect_SpeedDataOut()
{
	return _consumer._retn();
}
