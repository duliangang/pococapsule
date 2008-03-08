/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _audio_effect_h
#define _audio_effect_h

#include "sampleImpl.h"
#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

class POCO_EXPORT AudioDevice : public SDR::Controllable
{
	std::string		_name;

	// in ports
	SDR::PushPort*		_audioIn;
	SDR::PushPort*		_audioInRightDouble;
	SDR::PushPort*		_audioInLeftDouble;

	// out ports
	SDR::PushPort*		_audioOut;
	SDR::PushPort*		_audioOutLeftDouble;
	SDR::Log*		_logPort;

  public:
	AudioDevice(const char* name);
	~AudioDevice();
	const char* name() { return _name.c_str(); }

	// in ports
	SDR::PushPort* AudioIn() { return _audioIn; }
	SDR::PushPort* AudioInRightDouble() { return _audioInRightDouble; }
	SDR::PushPort* AudioInLeftDouble() { return _audioInLeftDouble; }

	// out ports
	void AudioOut(SDR::PushPort* port);
	void AudioOutLeftDouble(SDR::PushPort* port);
	void logPort(SDR::Log* log);
};

class POCO_EXPORT AudioEchoResource : public SDR::Controllable
{
	std::string	_name;

	// in port
	SDR::PushPort*	_audioIn;

	// out ports
	SDR::PushPort*	_audioOut;
	SDR::Log*	_logPort;

  public:
	AudioEchoResource(const char* name);
	~AudioEchoResource();
	const char* name() { return _name.c_str(); }
	
	// in port
	SDR::PushPort* AudioIn() { return _audioIn; }

	// out ports
	void AudioOut(SDR::PushPort* port);
	void logPort(SDR::Log* log);
};

class POCO_EXPORT AudioChorusResource : public SDR::Controllable
{
	std::string	_name;

	// in ports
	SDR::PushPort*	_audioIn;
		
	// out ports
	SDR::PushPort*	_audioOut;
	SDR::Log*	_logPort;

  public:
	AudioChorusResource(const char* name);
	~AudioChorusResource();

	const char* name() { return _name.c_str(); }

	// in port
	SDR::PushPort* AudioIn() { return _audioIn; }

	// out ports
	void AudioOut(SDR::PushPort* port);
	void logPort(SDR::Log* log);
};

class POCO_EXPORT AudioEffectController 
{
	std::string	_name;

	// in port

	// out ports
	SDR::Controllable*	_audioInControl;
	SDR::Controllable*	_audioOutControl;
	SDR::Controllable*	_audioEchoControl;
	SDR::Controllable*	_audioChorusControl;

	SDR::Log*		_logPort;

  public:
	AudioEffectController(const char* name);
	const char* name() { return _name.c_str(); }

	// out port
	void AudioInControl(SDR::Controllable* obj);
	void AudioOutControl(SDR::Controllable* obj);
	void AudioEchoControl(SDR::Controllable* obj);
	void AudioChorusControl(SDR::Controllable* obj);
	void logPort(SDR::Log* log);
};

#endif
