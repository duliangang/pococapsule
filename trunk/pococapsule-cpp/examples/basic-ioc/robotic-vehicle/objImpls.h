/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _obj_impls_h
#define _obj_impls_h

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows
 
class LogAndDisplay;
class ServoControl;
class ScanControl;
class SpeedDataConsumer;
class BumperDataConsumer;
class RangeScanDataConsumer;

#include <stdio.h>
#include <string>

class POCO_EXPORT LogAndDisplay 
{
	std::string 	_name;
  public:
	LogAndDisplay(const char* name) : _name(name) {}
	const char*	name() { return _name.c_str(); } 
};

class POCO_EXPORT ServoControl
{
	std::string	_name;
  public:
	ServoControl(const char* name) : _name(name) {}
	const char*	name() { return _name.c_str(); }
};

class POCO_EXPORT ScanControl 
{
	std::string	_name;
  public:
	ScanControl(const char* name) : _name(name) {}
	const char*	name() { return _name.c_str(); }
};

class POCO_EXPORT SpeedDataConsumer 
{
	std::string	_name;
  public:
	SpeedDataConsumer(const char* name) : _name(name) {}
	const char*	name() { return _name.c_str(); }
};

class POCO_EXPORT BumperDataConsumer 
{
	std::string	_name;
  public:
	BumperDataConsumer(const char* name) : _name(name) {}
	const char*	name() { return _name.c_str(); }
};

class POCO_EXPORT RangeScanDataConsumer 
{
	std::string	_name;
  public:
	RangeScanDataConsumer(const char* name) : _name(name) {}
	const char*	name() { return _name.c_str(); }
};

#endif
