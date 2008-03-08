/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "VehicleController.h"
#include "objImpls.h"

VehicleController::VehicleController(const char* name) : _name(name)
{
	char	buf[256];
	sprintf(buf, "LeftWheelDataIn sink reference of the %s", name);
	_left_wheel_data_in = new SpeedDataConsumer(buf);

	sprintf(buf, "RightWheelDataIn sink reference of the %s", name);
	_right_wheel_data_in = new SpeedDataConsumer(buf);

	sprintf(buf, "FrontBumperDataIn sink reference of the %s", name);
	_front_bumper_data_in = new BumperDataConsumer(buf);

	sprintf(buf, "RearBumperDataIn sink reference of the %s", name);
	_rear_bumper_data_in = new BumperDataConsumer(buf);

	sprintf(buf, "RangeScanDataIn sink reference of the %s", name);
	_range_scan_data_in = new RangeScanDataConsumer(buf);
}

void VehicleController::LeftWheelControlOut(ServoControl* servo)
{
	printf("[wire]: The %s is injected into the LeftWheelControlOut port of the %s\n", 
			servo->name(), name());
	_left_wheel = servo;
}

void VehicleController::RightWheelControlOut(ServoControl* servo)
{
	printf("[wire]: The %s is injected into the RightWheelControlOut port of the %s\n", 
			servo->name(), name());
	_right_wheel = servo;
}

void VehicleController::ScanCommandOut(ScanControl* scanner)
{
	printf("[wire]: The %s is injected into the ScanCommandOut port of the %s\n", 
			scanner->name(), name());
	_scanner = scanner;
}

void VehicleController::StatusOut(LogAndDisplay* log)
{
	printf("[wire]: The %s is injected into the StatusOut port of the %s\n", 
			log->name(), name());
	_log = log;
}

// sinks
SpeedDataConsumer* VehicleController::LeftWheelSpeedDataIn()
{
	return _left_wheel_data_in;
}

SpeedDataConsumer* VehicleController::RightWheelSpeedDataIn()
{
	return _right_wheel_data_in;
}

BumperDataConsumer* VehicleController::FrontBumperDataIn()
{
	return _front_bumper_data_in;
}

BumperDataConsumer* VehicleController::RearBumperDataIn()
{
	return _rear_bumper_data_in;
}

RangeScanDataConsumer* VehicleController::RangeScanDataIn()
{
	return _range_scan_data_in;
}
