/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include <pocoapp.h>
#include <stdio.h>

#ifdef USE_VBROKER
#include <corba.h>
#endif

#ifdef USE_TAO
#include <tao/corba.h>
#endif

int main(int argc, char** argv)
{
        POCO_AppEnv* env = POCO_AppContext::initDefaultAppEnv(argc, argv);

	try {
		//
		// deploy the server
		//
		POCO_AppContext* ctxt 
			= POCO_AppContext::create("setup.xml", "file");
	
		if( ctxt == NULL || !ctxt->initSingletons() ) {
			printf("Failure: %s\n", env->get_message());
			return -1;
		}

		//
		// run the server 
		//
		printf("Server is ready, with URL(s):\n");
		if( ctxt->getBean("pocomatic.dispatcher:my-orb") == NULL 
		 && env->has_error() ) {
			printf("Failure: %s\n", env->get_message());
			ctxt->terminate();
			return -1;
		}
	}
	catch(const CORBA::Exception& e) {
		printf("Exception: %s\n", e._name());
		return -1;
	}

	return 0;
}
