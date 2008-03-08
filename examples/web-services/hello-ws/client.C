#include <waspc/common.h>
#include <waspc/runtime/Runtime.h>

#include "hello.h"
#include <iostream>

int main(int argc,char **argv)
{
	WASP_Runtime::clientInitialize();

	try {
		WASP_Runtime::clientStart("conf/client.xml", NULL);

		HelloService proxy;

		if (argc != 1) {
			// use the user specified alternative address
			proxy.stub.setAddress(argv[1], NULL);
		} 

		std::string reply = proxy.hello("hello greeting from client");
		std::cout << "Client received a " << reply << std::endl;
	} 
	catch(WASP_Exception *exc) { 
		std::cerr << "Exception: " << exc->getCharMessage() << std::endl;
		delete exc; 
		return -1;
	}

	WASP_Runtime::clientTerminate();
	return 0;
}
