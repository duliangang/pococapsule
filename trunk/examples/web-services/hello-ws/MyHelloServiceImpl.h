#ifndef _hello_h
#define _hello_h

#include <waspc/config/config.h>
#include "helloImpl.h"
#include <iostream>

typedef HelloServiceImpl HelloServiceSkel; // HelloServiceImpl is WASPC generated skeleton.

class MyHelloServiceImpl : public HelloServiceSkel {
  public:
	std::string hello (const std::string & message) {
		std::cout <<  "Server received a " << message << std::endl;
		return "hello greeting from Server";
	}
};

#endif
