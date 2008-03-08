#include <pocoapp.h>

#include <string>
#include <iostream>

class MyErrorHandler : public POCO_AppEnv {
  public:
	MyErrorHandler(int argc, const char** argv) : POCO_AppEnv(argc, argv) {}
	void fatal_error(const char* msg) {
		throw std::string(msg);
	}
};

int main(int argc, char** argv)
{
	MyErrorHandler  env(argc, (const char**)argv);
	POCO_AppContext::setDefaultAppEnv(&env);

	try {
		POCO_AppContext* ctxt = POCO_AppContext::create("setup.xml", "file");
		ctxt->initSingletons();

		std::cout << "Server is ready" << std::endl;
		ctxt->getBean("pocomatic.dispatcher:my-ws");
	}
	catch(const std::string& e) {
		std::cout << "Failure: " << e << std::endl;
		return -1;
	}

	return 0;
}
