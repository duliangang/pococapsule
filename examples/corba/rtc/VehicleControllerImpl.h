/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _vehicle_controller_h
#define _vehicle_controller_h

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows
#include "sample_c.hh"

class POCO_EXPORT LogAndDisplayImpl : public sample::LogAndDisplay 
{
	CORBA::String_var	_name;
  public:
	RTC::ReturnCode_t initialize() { printf("[init]: the %s initialized\n", _name.in()); return 0; }
	RTC::ReturnCode_t finalize() { printf("[fini]: the %s finalized\n", _name.in()); CORBA::release(this); return 0; }
	LogAndDisplayImpl(const char* name) : _name(name) {}
	char*	name() { return CORBA::string_dup(_name.in()); } 
};

class POCO_EXPORT VehicleControllerImpl : public sample::VehicleController
{
	CORBA::String_var	_name;
	sample::ServoControl_var	_left_wheel;
	sample::ServoControl_var	_right_wheel;
	sample::ScanControl_var		_scanner;
	sample::LogAndDisplay_var	_log;

	sample::SpeedDataConsumer_var	_left_wheel_data_in;
	sample::SpeedDataConsumer_var	_right_wheel_data_in;
	sample::BumperDataConsumer_var	_front_bumper_data_in;
	sample::BumperDataConsumer_var	_rear_bumper_data_in;
	sample::RangeScanDataConsumer_var	_range_scan_data_in;

  public:
	VehicleControllerImpl(const char* name);
	char* name() { return CORBA::string_dup(_name.in()); }
	RTC::ReturnCode_t initialize() { printf("[init]: the %s is initialized\n", _name.in()); return 0; }
	RTC::ReturnCode_t finalize() { printf("[fini]: the %s is finalized\n", _name.in()); CORBA::release(this); return 0; }

	// receptacles
	RTC::ReturnCode_t connect_LeftWheelControlOut(sample::ServoControl_ptr servo);
	RTC::ReturnCode_t connect_RightWheelControlOut(sample::ServoControl_ptr servo);
        RTC::ReturnCode_t connect_ScanCommandOut(sample::ScanControl_ptr scanner);
        RTC::ReturnCode_t connect_StatusOut(sample::LogAndDisplay_ptr log);

	// sinks
	sample::SpeedDataConsumer_ptr get_consumer_LeftWheelSpeedDataIn();
	sample::SpeedDataConsumer_ptr get_consumer_RightWheelSpeedDataIn();
	sample::BumperDataConsumer_ptr get_consumer_FrontBumperDataIn();
	sample::BumperDataConsumer_ptr get_consumer_RearBumperDataIn();
	sample::RangeScanDataConsumer_ptr get_consumer_RangeScanDataIn();
};

#endif
