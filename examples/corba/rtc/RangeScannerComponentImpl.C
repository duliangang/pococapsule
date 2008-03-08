/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "RangeScannerComponentImpl.h"

class ScanControlImpl : public sample::ScanControl 
{
	CORBA::String_var	_name;
  public:
	ScanControlImpl(const char* name) : _name(name) {}
	char*	name() { return CORBA::string_dup(_name.in()); }
};

RangeScannerComponentImpl::RangeScannerComponentImpl(const char* name) : _name(name)
{ 
	char buf[256];
	sprintf(buf, "Control facet of %s", name);
	_control = new ScanControlImpl(buf); 
}

sample::ScanControl_ptr RangeScannerComponentImpl::provide_Control()
{
	return sample::ScanControl::_duplicate(_control.in());
}

RTC::ReturnCode_t RangeScannerComponentImpl::connect_RangeScanDataOut(sample::RangeScanDataConsumer_ptr consumer)
{
	CORBA::String_var consumer_name = consumer->name();
	printf("[wire]: The %s is connected to the RangeScanDataOut port of %s\n", 
			consumer_name.in(), _name.in());
	_consumer = sample::RangeScanDataConsumer::_duplicate(consumer);

	return 0; // RTC::OK;
}

sample::RangeScanDataConsumer_ptr RangeScannerComponentImpl::disconnect_RangeScanDataOut()
{
	return _consumer._retn();
}
