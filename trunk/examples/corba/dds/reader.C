/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include <pocoapp.h>
#include <stdio.h>
#include <string.h>

#ifdef USE_VBROKER
#include <corba.h>
#endif

#ifdef USE_TAO
#include <tao/corba.h>
#endif

static int validate_command_line(int argc, char** argv);

int main(int argc, char** argv) 
{
	if( validate_command_line(argc, argv) == -1 ) {
		return -1;
	}

	POCO_AppEnv* env = POCO_AppContext::initDefaultAppEnv(argc, argv);

	try {
		//
		// deploy the reader application 
		//
		POCO_AppContext* ctxt
			= POCO_AppContext::create("reader.xml", "file");

		if( ctxt == NULL || !ctxt->initSingletons() ) {
			printf("Failure: %s\n", env->get_message());
                        return -1;
                }

		//
		// start the dispatcher
		//
		printf("dds data reader is ready!\n");
		if( ctxt->getBean("pocomatic.dispatcher:my-orb") == NULL
		 && env->has_error() ) {
			printf("Failure: %s\n", env->get_message());
			ctxt->terminate();
			return -1;
		}
	}
	catch(const CORBA::Exception& e) {
		printf("Exceptin: %s\n", e._name());
		return -1;
	}
}

static void print_usage(const char*);
static const char* TOKEN = "NotificationService=";

static int validate_command_line(int argc, char** argv) 
{
	if( argc == 1 ) {
		print_usage(argv[0]);
		return -1;
	}

	// To ensure the notification service
	// url is configured in system property
	for(int i=1;i<argc-1;i++) {
		if( strcmp(argv[i], "-ORBInitRef") == 0
		 && strncmp(argv[i+1], TOKEN, strlen(TOKEN)) == 0 ) {
			return 0;
		}
	}

	print_usage(argv[0]);
	return -1;
}

static void print_usage(const char* cmd)
{
	printf(
        "usage:\n" 
        "   %s -ORBInitRef NotificationService=<url|ior>\n", cmd);
}
