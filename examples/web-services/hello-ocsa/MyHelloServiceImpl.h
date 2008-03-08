#ifndef _hello_h
#define _hello_h

#include <waspc/config/config.h>
#include "helloImpl.h"
#include <iostream>

typedef HelloServiceImpl HelloServiceSkel; // HelloServiceImpl is WASPC generated skeleton

class MyHelloServiceImpl : public HelloServiceSkel {
	std::string	_whoami;

  public:
	std::string hello (const std::string & message) {
		std::cout <<  "Server received a " << message << std::endl;
		std::string msg = "hello greeting from " + _whoami;

		return msg;
	}

	void whoami(const char* w) {
		_whoami = w;
	}
};

#endif
