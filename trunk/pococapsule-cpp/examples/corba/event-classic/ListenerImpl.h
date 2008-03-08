/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _my_listener_impl_h
# define _my_listener_impl_h

#include "MyTypedEvent_s.hh"

#ifdef USE_VBROKER
#include <CosNotification_s.hh>
#include <CosTypedEventComm_s.hh>
#include <CosNotifyComm_s.hh>
#endif

#ifdef USE_TAO
#include <orbsvcs/CosNotificationS.h>
#include <orbsvcs/CosTypedEventCommS.h>
#include <orbsvcs/CosNotifyCommS.h>
#endif

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

//
// Typed event listener. 
//

class POCO_EXPORT TypedEventListenerImpl
	: public POA_sample::MyTypedEvent,
	  public virtual PortableServer::RefCountServantBase 
{
  public:
	void hello(const char* greeting, const char* sender);
	void headlinenews(const sample::NewsBrief& news, const char* sender);
	void stockquote(const char* id, CORBA::Float quote);
};

//
// Structured event listener. 
//
class POCO_EXPORT StructuredEventListenerImpl
	: public POA_CosNotifyComm::StructuredPushConsumer,
	  public virtual PortableServer::RefCountServantBase 
{
  public:
	void push_structured_event(
		const CosNotification::StructuredEvent& event);

	void disconnect_structured_push_consumer() {}
	void offer_change(
		const CosNotification::EventTypeSeq&,
		const CosNotification::EventTypeSeq&) {}
};

//
// Sequence event listener. 
//
class POCO_EXPORT SequenceEventListenerImpl
	: public POA_CosNotifyComm::SequencePushConsumer,
	  public virtual PortableServer::RefCountServantBase 
{
  public:
	void push_structured_events(
		const CosNotification::EventBatch& events);

	void disconnect_sequence_push_consumer() {}
	void offer_change(
		const CosNotification::EventTypeSeq&,
		const CosNotification::EventTypeSeq&) {}
};

//
// Untyped event listener. 
//
class POCO_EXPORT UntypedEventListenerImpl
	: public POA_CosNotifyComm::PushConsumer,
	  public virtual PortableServer::RefCountServantBase
{
  public:
	void push(const CORBA::Any& event);

	void disconnect_push_consumer() {}
	void offer_change(
		const CosNotification::EventTypeSeq&,
		const CosNotification::EventTypeSeq&) {}
};

#endif
