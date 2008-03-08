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
		// Setup the specified event adapter
		//
		POCO_AppContext* ctxt
			= POCO_AppContext::create("publish.xml", "file");
	
		if( ctxt == NULL || !ctxt->initSingletons() ) {
			printf("Failure: %s\n", env->get_message());
			return -1;
		}
			
		//
		// init and setup the required event supplier adapter
		//
		char adapter_name[64];
		sprintf(adapter_name, "%s-event-adapter", argv[1]);
		if( ctxt->getBean(adapter_name) == NULL ) {
			printf("Failure: %s\n", env->get_message());
			ctxt->terminate();
			return -1;
		}
	
		//
		// cleanup
		//
		ctxt->terminate();
	}
	catch(const CORBA::Exception& e) {
		printf("Exception: %s\n", e._name());
		return -1;
	}

	return 0;
}

static void print_usage(const char*);
static const char* TOKEN = "NotificationService=";

static int validate_command_line(int argc, char** argv)
{
	if( argc == 1 ) {
		print_usage(argv[0]);
		return -1;
	}

	if( strcmp(argv[1], "typed") == 0 
	 || strcmp(argv[1], "untyped") == 0
	 || strcmp(argv[1], "structured") == 0
	 || strcmp(argv[1], "sequence") == 0 ) {
		for(int i=2;i<argc-1;i++) {
			if( strcmp(argv[i], "-ORBInitRef") == 0
			 && strncmp(argv[i+1], TOKEN, strlen(TOKEN)) == 0 ) {
				return 0;
			}
		}
	}

	print_usage(argv[0]);
	return -1;
}

static void print_usage(const char* cmd) 
{
	printf(
	"usage:\n"
	"   %s (typed|untyped|structured|sequence|) \\\n"
	"           -ORBInitRef NotificationService=<url|ior>\n", cmd);
}
