/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _fm_trans_recv_h
#define _fm_trans_recv_h

#include "sampleImpl.h"
#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

class POCO_EXPORT DemodulationFMResource : public SDR::Controllable
{
	std::string	_name;

	// in port
	SDR::PushPort*	_doubleSeqIn;

	// out ports
	SDR::PushPort*	_doubleSeqOut;
	SDR::Log*	_logPort;

  public:
	DemodulationFMResource(const char* name);
	~DemodulationFMResource();
	const char* name() { return _name.c_str(); }

	// in port
	SDR::PushPort* DoubleSeqIn() { return _doubleSeqIn; }

	// out ports
	void DoubleSeqOut(SDR::PushPort* port);
	void logPort(SDR::Log* log);
};

class POCO_EXPORT ModulationFMResource : public SDR::Controllable
{
	std::string	_name;

	// in port
	SDR::PushPort*	_doubleSeqIn;

	// out ports
	SDR::PushPort*	_doubleSeqOut;
	SDR::Log*	_logPort;

  public:
	ModulationFMResource(const char* name);
	~ModulationFMResource();
	const char* name() { return _name.c_str(); }

	// in port
	SDR::PushPort* DoubleSeqIn() { return _doubleSeqIn; }

	// out ports
	void DoubleSeqOut(SDR::PushPort* port);
	void logPort(SDR::Log* log);
};

class POCO_EXPORT FMTransmitterReceiverAssemblyController
{
	std::string	_name;

	// in port

	// out ports
	SDR::Controllable*	_modulationFMControl;
	SDR::Controllable*	_demodulationFMControl;
	SDR::Controllable*	_outputDeviceControl;
	SDR::Controllable*	_inputDeviceControl;
	SDR::Log*		_logPort;

  public:
	FMTransmitterReceiverAssemblyController(const char* name);
	const char* name() { return _name.c_str(); }

	// out ports
	void ModulationFMControl(SDR::Controllable* obj);
	void DemodulationFMControl(SDR::Controllable* obj);
	void OutputDeviceControl(SDR::Controllable* obj);
	void InputDeviceControl(SDR::Controllable* obj);
	void logPort(SDR::Log* log);
};

#endif
