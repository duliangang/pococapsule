/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "AudioEffect.h"
#include <stdio.h>

//
// AudioDevice
//
AudioDevice::AudioDevice(const char* name) 
	: _name(name) 
{
	printf("[init]: The %s component is instantiated\n", name);

	char	buf[256];
	sprintf(buf, "AudioIn push-in port of %s", name); 
	_audioIn = new PushPortImpl(buf);

	sprintf(buf, "AudioInRightDouble push-in port of the %s", name);
	_audioInRightDouble = new PushPortImpl(buf);

	sprintf(buf, "AudioInLeftDouble push-in port of the %s", name);
	_audioInLeftDouble = new PushPortImpl(buf);
}

AudioDevice::~AudioDevice() 
{
	delete _audioIn;
	delete _audioInRightDouble;
	delete _audioInLeftDouble;
}

void AudioDevice::AudioOut(SDR::PushPort* port)
{
	if( port != NULL ) {
		printf("[wire]: The %s is connected to the AudioOut port of the %s\n",
			port->name(), _name.c_str()); 
	}

	_audioOut = port;
}

void AudioDevice::AudioOutLeftDouble(SDR::PushPort* port)
{
	if( port != NULL ) {
		printf("[wire]: The %s is subscribed to the AudioOutLeftDouble port of the %s\n",
			port->name(), _name.c_str()); 
	}

	_audioOutLeftDouble = port; // or put in a port array
}

void AudioDevice::logPort(SDR::Log* log)
{
	if( log != NULL ) {
		printf("[wire]: The log service is connected to the logPort of the %s\n", _name.c_str());
	}

	_logPort = log;
}

//
// EchoResource
//
AudioEchoResource::AudioEchoResource(const char* name) 
	: _name(name) {
	printf("[init]: The %s component is instantiated\n", name);

	char buf[256];
	sprintf(buf, "AudioIn push-in port of the %s", name);
	_audioIn = new PushPortImpl(buf);
}

AudioEchoResource::~AudioEchoResource()
{
	delete _audioIn;
}

void AudioEchoResource::AudioOut(SDR::PushPort* port)
{
	if( port != NULL ) {
		printf("[wire]: The %s is connected to the AudioOut port of the %s\n",
			port->name(), _name.c_str());
	}

	_audioOut = port;
}

void AudioEchoResource::logPort(SDR::Log* log)
{
	if( log != NULL ) {
		printf("[wire]: The log service is connected the logPort of the %s\n", _name.c_str());
	}

	_logPort = log;
}

//
// Audio Chorus Resource
//
AudioChorusResource::AudioChorusResource(const char* name)
	: _name(name) {
	printf("[init]: The %s component is instantiated\n", name);

	char buf[256];
	sprintf(buf, "AudioIn push-in port of the %s", name);
	_audioIn = new PushPortImpl(buf);
}

AudioChorusResource::~AudioChorusResource()
{
	delete _audioIn;
}

void AudioChorusResource::AudioOut(SDR::PushPort* port) {
	if( port != NULL ) {
		printf("[wire]: The %s is connected to the AudioOut port of the %s\n", port->name(), _name.c_str());
	}

	_audioOut = port;
}

void AudioChorusResource::logPort(SDR::Log* log)
{
	if( log != NULL ) {
		printf("[wire]: The log service is connected the logPort of the %s\n", _name.c_str());
	}

	_logPort = log;
}

//
// Audio Effect Controller
//
AudioEffectController::AudioEffectController(const char* name)
	: _name(name) {
	printf("[init]: The %s component is instantiated\n", name);
}

void AudioEffectController::AudioInControl(SDR::Controllable* obj)
{
	if( obj != NULL ) {
		printf("[wire]: The %s is connected to the AudioInControl port of the %s\n",
				obj->name(), _name.c_str());
	}

	_audioInControl = obj;
}

void AudioEffectController::AudioOutControl(SDR::Controllable* obj)
{
	if( obj != NULL ) {
		printf("[wire]: The %s is connected to the AudioOutControl port of the %s\n",
				obj->name(), _name.c_str());
	}

	_audioOutControl = obj;
}
	
void AudioEffectController::AudioEchoControl(SDR::Controllable* obj)
{
	if( obj != NULL ) {
		printf("[wire]: The %s is connected to the AudioEchoControl port of the %s\n",
				obj->name(), _name.c_str());
	}

	_audioEchoControl = obj;
}

void AudioEffectController::AudioChorusControl(SDR::Controllable* obj) {
	if( obj != NULL ) {
		printf("[wire]: The %s is connected to the AudioChorusControl port of the %s\n",
				obj->name(), _name.c_str());
	}

	_audioChorusControl = obj;
}

void AudioEffectController::logPort(SDR::Log* log)
{
	if( log != NULL ) {
		printf("[wire]: The log service is connected to the logPort of the %s\n", _name.c_str());
	}

	_logPort = log;
}
