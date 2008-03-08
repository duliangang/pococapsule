/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "pocoapp.h"
#include <stdio.h>

#include "Node.h"

int main(int argc, char** argv)
{
	POCO_AppEnv* env = POCO_AppContext::initDefaultAppEnv(argc, argv);

	POCO_AppContext* ctx = POCO_AppContext::create("setup.xml", "file");

	if( ctx == NULL ) {
		printf("failure: %s\n", env->get_message());
		return -1;
	}

	Node* head = (Node*)ctx->getBean("upstream", "Node");

	head->push("start", 0);

	return 0;
}
