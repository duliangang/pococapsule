/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "VehicleControllerImpl.h"

class SpeedDataConsumerImpl : public sample::SpeedDataConsumer 
{
	CORBA::String_var	_name;
  public:
	SpeedDataConsumerImpl(const char* name) : _name(name) {}
	char*	name() { return CORBA::string_dup(_name.in()); }
};

class BumperDataConsumerImpl : public sample::BumperDataConsumer 
{
	CORBA::String_var	_name;
  public:
	BumperDataConsumerImpl(const char* name) : _name(name) {}
	char*	name() { return CORBA::string_dup(_name.in()); }
};

class RangeScanDataConsumerImpl : public sample::RangeScanDataConsumer
{
	CORBA::String_var	_name;
  public:
	RangeScanDataConsumerImpl(const char* name) : _name(name) {}
	char*	name() { return CORBA::string_dup(_name.in()); }
};

VehicleControllerImpl::VehicleControllerImpl(const char* name) : _name(name)
{
	char	buf[256];
	sprintf(buf, "LeftWheelDataIn sink port of the %s", name);
	_left_wheel_data_in = new SpeedDataConsumerImpl(buf);

	sprintf(buf, "RightWheelDataIn sink port of the %s", name);
	_right_wheel_data_in = new SpeedDataConsumerImpl(buf);

	sprintf(buf, "FrontBumperDataIn sink port of the %s", name);
	_front_bumper_data_in = new BumperDataConsumerImpl(buf);

	sprintf(buf, "RearBumperDataIn sink port of the %s", name);
	_rear_bumper_data_in = new BumperDataConsumerImpl(buf);

	sprintf(buf, "RangeScanDataIn sink port of the %s", name);
	_range_scan_data_in = new RangeScanDataConsumerImpl(buf);
}

RTC::ReturnCode_t VehicleControllerImpl::connect_LeftWheelControlOut(sample::ServoControl_ptr servo)
{
	CORBA::String_var servo_name = servo->name();
	printf("[wire]: The %s is connected to the LeftWheelControlOut port of the %s\n", 
			servo_name.in(), _name.in());
	_left_wheel = sample::ServoControl::_duplicate(servo);

	return 0; // RTC::OK;
}

RTC::ReturnCode_t VehicleControllerImpl::connect_RightWheelControlOut(sample::ServoControl_ptr servo)
{
	CORBA::String_var servo_name = servo->name();
	printf("[wire]: The %s is connected to the RightWheelControlOut port of the %s\n", 
			servo_name.in(), _name.in());
	_right_wheel = sample::ServoControl::_duplicate(servo);

	return 0; // RTC::OK;
}

RTC::ReturnCode_t VehicleControllerImpl::connect_ScanCommandOut(sample::ScanControl_ptr scanner)
{
	CORBA::String_var scanner_name = scanner->name();
	printf("[wire]: The %s is connected to the ScanCommandOut port of the %s\n", 
			scanner_name.in(), _name.in());
	_scanner = sample::ScanControl::_duplicate(scanner);

	return 0; // RTC::OK;
}

RTC::ReturnCode_t VehicleControllerImpl::connect_StatusOut(sample::LogAndDisplay_ptr log)
{
	CORBA::String_var log_name = log->name();
	printf("[wire]: The %s is connected to the StatusOut port of the %s\n", 
			log_name.in(), _name.in());
	_log = sample::LogAndDisplay::_duplicate(log);

	return 0; // RTC::OK;
}

// sinks
sample::SpeedDataConsumer_ptr VehicleControllerImpl::get_consumer_LeftWheelSpeedDataIn()
{
	return sample::SpeedDataConsumer::_duplicate(_left_wheel_data_in.in());
}

sample::SpeedDataConsumer_ptr VehicleControllerImpl::get_consumer_RightWheelSpeedDataIn()
{
	return sample::SpeedDataConsumer::_duplicate(_right_wheel_data_in.in());
}

sample::BumperDataConsumer_ptr VehicleControllerImpl::get_consumer_FrontBumperDataIn()
{
	return sample::BumperDataConsumer::_duplicate(_front_bumper_data_in.in());
}

sample::BumperDataConsumer_ptr VehicleControllerImpl::get_consumer_RearBumperDataIn()
{
	return sample::BumperDataConsumer::_duplicate(_rear_bumper_data_in.in());
}

sample::RangeScanDataConsumer_ptr VehicleControllerImpl::get_consumer_RangeScanDataIn()
{
	return sample::RangeScanDataConsumer::_duplicate(_range_scan_data_in.in());
}
