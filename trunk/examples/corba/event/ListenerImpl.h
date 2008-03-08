/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _my_listener_impl_h
# define _my_listener_impl_h

#include "MyTypedEvent_s.hh"

#ifdef USE_VBROKER
#include <CosNotification_c.hh>
#endif

#ifdef USE_TAO
#include <orbsvcs/CosNotificationC.h>
#endif

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

//
// A native implementation provides signatures to receive 
// all types (typed, structured, sequence, or untyped) of events.
//
// This implementation is not a POA servant, therefore, is going 
// to be deployed with kind="native".
//

class POCO_EXPORT EventListenerImpl 
	// : public POA_sample::MyTypedEvent, public PortableServer::RefCountServantBase 
{
  public:
	//
	// Typed event push-in callback operations
	//
	void hello(const char* greeting, const char* sender);
	void headlinenews(const sample::NewsBrief& news, const char* sender);
	void stockquote(const char* id, CORBA::Float quote);

	//
	// Structured event push-in callback operation
	//
	void push_structured_event(
		const CosNotification::StructuredEvent& event);

	//
	// Sequence event push-in callbak operation
	//
	void push_structured_events(
		const CosNotification::EventBatch& events);

	//
	// Untyped event push-in callback operation. 
	//
	void push(const CORBA::Any& event);
};

#endif
