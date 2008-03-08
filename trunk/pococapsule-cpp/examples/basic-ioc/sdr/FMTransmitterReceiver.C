/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "FMTransmitterReceiver.h"

//
// DemodulationFMResource
//
DemodulationFMResource::DemodulationFMResource(const char* name)
	: _name(name) {
	printf("[init]: The %s component is instantiated\n", name);

	char buf[256];
	sprintf(buf, "DoubleSeqIn push-in port of %s", name);
	_doubleSeqIn = new PushPortImpl(buf);
}

DemodulationFMResource::~DemodulationFMResource() {
	delete _doubleSeqIn;
}

void DemodulationFMResource::DoubleSeqOut(SDR::PushPort* port) {
	if( port != NULL ) {
		printf("[wire]: The %s is connected to the DoubleSeqOut port of the %s\n",
			port->name(), _name.c_str());
	}

	_doubleSeqOut = port;
}

void DemodulationFMResource::logPort(SDR::Log* log) {
	if( log != NULL ) {
		printf("[wire]: The log service is connected to the logPort of the %s\n", _name.c_str());
	}

	_logPort = log;
}

//
// ModulationFMResource
//
ModulationFMResource::ModulationFMResource(const char* name)
	: _name(name) {
	printf("[init]: The %s component is instantiated\n", name);

	char buf[256];
	sprintf(buf, "DoubleSeqIn push-in port of %s", name);
	_doubleSeqIn = new PushPortImpl(buf);
}

ModulationFMResource::~ModulationFMResource() {
	delete _doubleSeqIn;
}

void ModulationFMResource::DoubleSeqOut(SDR::PushPort* port) {
	if( port != NULL ) {
		printf("[wire]: The %s is connected to the DoubleSeqOut port of the %s\n",
			port->name(), _name.c_str());
	}

	_doubleSeqOut = port;
}

void ModulationFMResource::logPort(SDR::Log* log) {
	if( log != NULL ) {
		printf("[wire]: The log service is connected to the logPort of the %s\n",
				_name.c_str());
	}

	_logPort = log;
}

//
// FMTransmitterReceiverAssemblyController
//
FMTransmitterReceiverAssemblyController::FMTransmitterReceiverAssemblyController(const char* name)
	: _name(name) {
	printf("[init]: The %s component is instantiated\n", name);
}

void FMTransmitterReceiverAssemblyController::ModulationFMControl(SDR::Controllable* obj) 
{
	if( obj != NULL ) {
		printf("[wire]: The %s is connected to the ModulationFMControl port of the %s\n",
				obj->name(), _name.c_str());
	}

	_modulationFMControl = obj;
}

void FMTransmitterReceiverAssemblyController::DemodulationFMControl(SDR::Controllable* obj) 
{
	if( obj != NULL ) {
		printf("[wire]: The %s is connected to the DemodulationFMControl port of the %s\n",
				obj->name(), _name.c_str());
	}

	_demodulationFMControl = obj;
}

void FMTransmitterReceiverAssemblyController::OutputDeviceControl(SDR::Controllable* obj) 
{
	if( obj != NULL ) {
		printf("[wire]: The %s is connected to the OutputDeviceControl port of the %s\n",
				obj->name(), _name.c_str());
	}

	_outputDeviceControl = obj;
}

void FMTransmitterReceiverAssemblyController::InputDeviceControl(SDR::Controllable* obj)
{
	if( obj != NULL ) {
		printf("[wire]: The %s is connected to the InputDeviceControl port of the %s\n",
				obj->name(), _name.c_str());
	}

	_inputDeviceControl = obj;
}

void FMTransmitterReceiverAssemblyController::logPort(SDR::Log* log) {
	if( log != NULL ) {
		printf("[wire]: The log service is connected to the logPort of the %s\n", _name.c_str());
	}

	_logPort = log;
}
