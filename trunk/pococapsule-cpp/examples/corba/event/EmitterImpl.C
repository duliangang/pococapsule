/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "EmitterImpl.h"
#include <stdio.h>

# include "MyTypedEvent_c.hh"

void EventEmitterImpl::connect_typed_push_consumer(
	CosTypedEventComm::TypedPushConsumer_ptr consumer) 
{
	try {
		// get the user defined event interface facet
		CORBA::Object_var ref = consumer->get_typed_consumer();
		sample::MyTypedEvent_var tconsumer = sample::MyTypedEvent::_narrow(ref.in());

		for(int i=0; i<4; i++) {
			// send a hello event
			tconsumer->hello("Greeting to everyone", "info service"); 

			// send a news event
			sample::NewsBrief news;
			news.subject = (const char*)"new PocoCapsule release";
			news.message = (const char*)"IoC + DSL + CBD for C++";
			tconsumer->headlinenews(news, "info service");

			// send a stock quote event
			tconsumer->stockquote("POCO", (CORBA::Float)120.00);
		}

		// flush buffered events
		tconsumer->_non_existent();

		printf("---- pushed 12 typed events to channel ---\n");
	}
	catch(const CORBA::Exception& e) {
		printf("connect_typed_push_consumer() exception: %s\n",
			e._name());
	}
}

static void forge_event(CosNotification::StructuredEvent& event);

void EventEmitterImpl::connect_structured_push_consumer(
	CosNotifyComm::StructuredPushConsumer_ptr consumer)
{
	try {
		CosNotification::StructuredEvent event;
		// push 3 structured events
		for(int i=0;i<3;i++) {
			forge_event(event);
			consumer->push_structured_event(event);
		}

		printf("---- pushed 3 structured events to channel ---\n");
	}
	catch(const CORBA::Exception& e) {
		printf("connect_structured_push_consumer() exception: %s\n",
			e._name());
	}
}

static void forge_event(CosNotification::EventBatch& events);

void EventEmitterImpl::connect_sequence_push_consumer(
	CosNotifyComm::SequencePushConsumer_ptr consumer) 
{
	try {
		// push one sequence events
		for(int i=0;i<1;i++) {
			CosNotification::EventBatch events;
			events.length(3);
			forge_event(events);
			consumer->push_structured_events(events);
		}
		printf("---- pushed 1 batch(s) of 3 events sequence to channel ---\n");
	}
	catch(const CORBA::Exception& e) {
		printf("connect_sequence_push_consumer() exception %s\n",
			e._name());
	}
}

static void forge_event(CORBA::Any&);

void EventEmitterImpl::connect_any_push_consumer(
	CosEventComm::PushConsumer_ptr consumer)
{
	try {
		// push 8 untyped events
		for(int i=0;i<8;i++) {
			CORBA::Any event;
			forge_event(event);
			consumer->push(event);
		}
		printf("---- pushed 8 untyped events to channel ---\n");
	}
	catch(const CORBA::Exception& e) {
		printf("connect_any_push_consumer() exception: %s\n",
			e._name());
	}
}

static void forge_event(CosNotification::StructuredEvent& event) 
{
	static int count = 0;

	count++;
	// forge fixed_header.event_type
	event.header.fixed_header.event_type.domain_name = (const char*)"pocomatic";
	event.header.fixed_header.event_type.type_name = (const char*)"com";

	// forge fixed_header.event_name
	switch(count%3) {
		case 0:
		event.header.fixed_header.event_name = (const char*)"hello";
		break;

		case 1:
		event.header.fixed_header.event_name = (const char*)"headlinenews";
		break;

		case 2:
		event.header.fixed_header.event_name = (const char*)"stockquote";
		break;
	}
 

	// forge header.variable_header
	int len = 1;
	event.header.variable_header.length(len);
	for(int i=0; i<len; i++) {
		event.header.variable_header[i].name = (const char*)"time";
		event.header.variable_header[i].value <<= (const char*)"12:12:59";
	}

	// forge filterable_data
	len = 1;
	event.filterable_data.length(len);
	for(int i=0; i<len; i++) {
		event.filterable_data[i].name = (const char*)"hello";
		event.filterable_data[i].value <<= (const char*)"from provider";
	}

	// forge remainder_of_body
	char body[64];
	sprintf(body, "structured event count %d", count);
	event.remainder_of_body <<= (const char*)body;
}

static void forge_event(CosNotification::EventBatch& events)
{
	for(unsigned long i=0; i<events.length(); i++) {
		forge_event(events[i]);
	}
}

static void forge_event(CORBA::Any& event)
{
	static int count = 0;

	count++;

	char msg[128];
	sprintf(msg, "untyped event %d from provider", count);

	event <<= (const char*)msg;
}
