/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _range_scanner_component_impl_h
#define _range_scanner_component_impl_h

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows
#include "sample_c.hh"

class POCO_EXPORT RangeScannerComponentImpl : public sample::RangeScannerComponent
{
	CORBA::String_var	_name;
	sample::ScanControl_var		_control;
	sample::RangeScanDataConsumer_var	_consumer;

  public:
	RangeScannerComponentImpl(const char* name);
	char* name() { return CORBA::string_dup(_name.in()); }
	RTC::ReturnCode_t initialize() { printf("[init]: the %s initialized\n", _name.in()); return 0; }
	RTC::ReturnCode_t finalize() { printf("[fini]: the %s finalized\n", _name.in()); CORBA::release(this); return 0; }

	// facets
	sample::ScanControl_ptr provide_Control();

	// emitters
	RTC::ReturnCode_t connect_RangeScanDataOut(sample::RangeScanDataConsumer_ptr consumer);
	sample::RangeScanDataConsumer_ptr disconnect_RangeScanDataOut();
};

#endif
