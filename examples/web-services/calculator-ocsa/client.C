#include <waspc/common.h>
#include <waspc/runtime/Runtime.h>

#include "calculator.h"
#include <iostream>

int main(int argc,char **argv)
{
	WASP_Runtime::clientInitialize();

	try {
		WASP_Runtime::clientStart("conf/client.xml", NULL);

		CalculatorPort proxy;

		if (argc != 1) {
			// use the user specified alternative address
			proxy.stub.setAddress(argv[1], NULL);
		} 

		std::cout << "add(1.23, 2.45) = ";
		float reply = proxy.add(1.23, 2.45);
		std::cout << reply << std::endl;

		std::cout << "sub(1.23, 2.45) = ";
		reply = proxy.sub(1.23, 2.45);
		std::cout << reply << std::endl;

		std::cout << "mul(1.23, 2.45) = ";
		reply = proxy.mul(1.23, 2.45);
		std::cout << reply << std::endl;

		std::cout << "div(1.23, 2.45) = ";
		reply = proxy.div_1(1.23, 2.45);
		std::cout << reply << std::endl;
		
	} 
	catch(WASP_Exception *exc) { 
		std::cerr << "Exception: " << exc->getCharMessage() << std::endl;
		delete exc; 
		return -1;
	}

	WASP_Runtime::clientTerminate();
	return 0;
}
