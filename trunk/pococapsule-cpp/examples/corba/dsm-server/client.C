/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include <stdio.h>

#include "Greeting_c.hh"

int main(int argc, char** argv)
{
	if( argc == 1 ) {
		printf("usage:\n\t%s <server_url>\n", argv[0]);
		return -1;
	}

	try {
		CORBA::ORB_ptr orb = CORBA::ORB_init(argc, argv);

		//
		// destringifying the URL (i.e. argv[1]) to get the server's ref
		//
		CORBA::Object_var ref = orb->string_to_object(argv[1]);
		sample::Greeting_var server = sample::Greeting::_narrow(ref.in());

		//
		// Sending 10 greetings to the server and get replies.
		//
		for(int i=0;i<10;i++) {
			char msg[128];
			sprintf(msg, "hello greeting %d from client", i);
			CORBA::String_var reply = server->hello(msg);

			printf("Client received a reply: %s\n", reply.in());
		}
	}
	catch(const CORBA::Exception& e) {
		printf("Exception: %s\n", e._name());
		return -1;
    	}

	return 0;
}
