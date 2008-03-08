/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _vehicle_controller_h
#define _vehicle_controller_h

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows
#include "objImpls.h"

class POCO_EXPORT VehicleController
{
	std::string	_name;
	ServoControl*	_left_wheel;
	ServoControl*	_right_wheel;
	ScanControl*		_scanner;
	LogAndDisplay*	_log;

	SpeedDataConsumer*	_left_wheel_data_in;
	SpeedDataConsumer*	_right_wheel_data_in;
	BumperDataConsumer*	_front_bumper_data_in;
	BumperDataConsumer*	_rear_bumper_data_in;
	RangeScanDataConsumer*	_range_scan_data_in;

  public:
	VehicleController(const char* name);
	const char* name() { return _name.c_str(); }

	// receptacles
	void LeftWheelControlOut(ServoControl* servo);
	void RightWheelControlOut(ServoControl* servo);
        void ScanCommandOut(ScanControl* scanner);
        void StatusOut(LogAndDisplay* log);

	// sinks
	SpeedDataConsumer* LeftWheelSpeedDataIn();
	SpeedDataConsumer* RightWheelSpeedDataIn();
	BumperDataConsumer* FrontBumperDataIn();
	BumperDataConsumer* RearBumperDataIn();
	RangeScanDataConsumer* RangeScanDataIn();
};

#endif
