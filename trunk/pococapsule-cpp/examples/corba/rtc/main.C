/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "pocoapp.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	POCO_AppEnv* env = POCO_AppContext::initDefaultAppEnv(argc, argv);

	POCO_AppContext* ctxt = POCO_AppContext::create("setup.xml", "file");

	if( ctxt == NULL || !ctxt->initSingletons() ) {
		printf("failure: %s\n", env->get_message());
		return -1;
	}

	printf("The robotic vehicle is ready to work .................\n");

	printf("Terminating the robotic vehicle now ..................\n");

	ctxt->terminate();
	ctxt->destroy();

	return 0;
}
