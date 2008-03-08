/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _sdr_h
#define _sdr_h

class SDR {
  public:
	class PushPort {
	  public:
		virtual ~PushPort() {}
		virtual const char*	name() = 0;
		// pushXXX() methods here ...
	};
	
	class Controllable {
	  public:
		virtual ~Controllable() {}
		virtual const char*	name() = 0;
		// controlling methods here ...
	};
	
	class Log {
	  public:
		// logging methods here
	};
};

#endif
