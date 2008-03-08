/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _GreetingImpl_h_
# define _GreetingImpl_h_

#include "Greeting_s.hh"

#ifdef USE_TAO
# include <tao/PortableServer/ServantActivatorA.h>
# include <tao/PortableServer/ServantLocatorA.h>
#endif

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows
//
// Server's servant bean implemenation.
//
class POCO_EXPORT GreetingImpl
	: public POA_sample::Greeting,
	  public virtual PortableServer::RefCountServantBase 
{
	CORBA::String_var _whoami;

   public:
	GreetingImpl(const char* who);
	char* hello(const char* msg);
};

//
// Server's servant activator implementation
//
class POCO_EXPORT ServantActivatorImpl : public PortableServer::ServantActivator {
   public:
	PortableServer::Servant incarnate(
		const PortableServer::ObjectId&	oid,
		PortableServer::POA_ptr		poa);

	void etherealize(
		const PortableServer::ObjectId&	oid,
		PortableServer::POA_ptr		poa,
		PortableServer::Servant		servant,
		CORBA::Boolean			cleanup_in_progress,
		CORBA::Boolean			remaining_activations);
};

//
// Server's servant locator implementation
//
class POCO_EXPORT ServantLocatorImpl : public PortableServer::ServantLocator {
	PortableServer::ServantBase_var _servant_array[5];
	int _round_robin_idx;

  public:
	ServantLocatorImpl();

	PortableServer::Servant preinvoke(
		const PortableServer::ObjectId&	oid,
		PortableServer::POA_ptr		poa,
		const char*			operation,
		PortableServer::ServantLocator::Cookie& cookie);

	void postinvoke(
		const PortableServer::ObjectId&	oid,
		PortableServer::POA_ptr		poa,
		const char*			operation,
		PortableServer::ServantLocator::Cookie cookie,
		PortableServer::Servant		servant);
};

#endif
