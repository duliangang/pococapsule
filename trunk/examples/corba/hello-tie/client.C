/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include <stdio.h>

#include "Greeting_c.hh"

int main(int argc, char** argv) {
	if( argc == 1 ) {
		printf("usage:\n\t%s <server_url>\n", argv[0]);
		return -1;
	}

	try {
      		CORBA::ORB_ptr orb = CORBA::ORB_init(argc, argv);

		//
		// destringify the URL (i.e. args[1]) to a programming object
		//
		CORBA::Object_var ref = orb->string_to_object(argv[1]);
		sample::Greeting_var server = sample::Greeting::_narrow(ref.in());

		//
		// invoke the server and get reply.
		//
		CORBA::String_var reply = server->hello("hello greeting from Client");

		printf("Client received a %s\n", reply.in());
	}
	catch(const CORBA::Exception& e) {
		printf("Exception: %s\n", e._name());
		return -1;
	}

	return 0;
}
