/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include <stdio.h>
#include <string.h>

#include "GreetingImpl.h"

GreetingImpl::GreetingImpl(const char* who) 
{
	printf("------- Allocated a GreetingImpl instance as %s --------\n", who);
	_whoami = who;
}

char* GreetingImpl::hello(const char* msg)
{
	printf("%s received a %s\n", _whoami.in(), msg);
	char reply[128];
	strcpy(reply, "hello greeting from ");
	strcat(reply, _whoami.in());
	return CORBA::string_dup(reply);
}

PortableServer::Servant ServantActivatorImpl::incarnate(
		const PortableServer::ObjectId&	oid,
		PortableServer::POA_ptr		poa)
{
	printf("------- ServantActivator.incarnate() --------\n");
	return new GreetingImpl("incarnated-servant");
}

void ServantActivatorImpl::etherealize(
		const PortableServer::ObjectId&	oid,
		PortableServer::POA_ptr		poa,
		PortableServer::Servant		servant,
		CORBA::Boolean			cleanup_in_progress,
		CORBA::Boolean			remaining_activations)
{
	printf("------- ServantActivator.etherealize() --------\n");
	servant->_remove_ref();
}

ServantLocatorImpl::ServantLocatorImpl()
{
	_round_robin_idx = 0;
	for(int i=0; i<5; i++) {
		char name[128];
		sprintf(name, "servant-%d-for-locator", i);
		_servant_array[i] = new GreetingImpl(name);
	}
}

PortableServer::Servant ServantLocatorImpl::preinvoke(
		const PortableServer::ObjectId&	oid,
		PortableServer::POA_ptr		poa,
		const char*			operation,
		PortableServer::ServantLocator::Cookie& cookie)
{
	printf("------- ServantLocator.preinvoke(.., op='%s', .. ) "
			"--------\n", operation);
	PortableServer::Servant servant 
			= _servant_array[(_round_robin_idx++)%5].in();
	servant->_add_ref();
	return servant;
}

void ServantLocatorImpl::postinvoke(
		const PortableServer::ObjectId&	oid,
		PortableServer::POA_ptr		poa,
		const char*			operation,
		PortableServer::ServantLocator::Cookie cookie,
		PortableServer::Servant		servant)
{
	printf("------- ServantLocator.postinvoke(.., op='%s', .. ) "
			"--------\n", operation);
	servant->_remove_ref();
}
