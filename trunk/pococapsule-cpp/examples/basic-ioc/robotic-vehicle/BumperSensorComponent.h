/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _bumper_sensor_component_impl_h
#define _bumper_sensor_component_impl_h

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows
#include <string>
#include "objImpls.h"

class POCO_EXPORT BumperSensorComponent
{
	std::string 			_name;
	BumperDataConsumer*		_consumer;

  public:
	BumperSensorComponent(const char* name);
	const char* name() { return _name.c_str(); }

	// emitters
	void SensorDataOut(BumperDataConsumer* consumer);
};

#endif
