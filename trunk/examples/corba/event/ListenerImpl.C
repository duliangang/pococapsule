/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include <stdio.h>

#include "ListenerImpl.h"

void EventListenerImpl::hello(const char* greeting, const char* sender)
{
	printf("received a hello '%s' event from %s\n", greeting, sender);
}

void EventListenerImpl::headlinenews(
			const sample::NewsBrief& news, const char* sender)
{
	printf("received a headline news: {'%s', '%s'} event from %s\n",
			news.subject.in(), news.message.in(), sender);
}

void EventListenerImpl::stockquote(const char* id, CORBA::Float quote)
{
	printf("received a stockquote: id=%s, quote=%f event\n", id, quote);
}

static void print_event(const CosNotification::StructuredEvent&);

void EventListenerImpl::push_structured_event(
		const CosNotification::StructuredEvent& event)
{
	printf("---- received a structured event ----\n");
	print_event(event);
}

void EventListenerImpl::push_structured_events(
		const CosNotification::EventBatch& events )
{
	printf("---- received structured events in sequence ----\n");
	for(unsigned long i=0;i<events.length();i++) {
		print_event(events[i]);
	}
} 

static void print_event(const CORBA::Any& event);

void EventListenerImpl::push(const CORBA::Any& event)
{
	printf("---- received untyped event ----\n");
	print_event(event);
}

#include <iostream>

static void print_event(const CosNotification::StructuredEvent& event)
{
	const char* ptr;
	CORBA::Any::to_string   extr(ptr, 0UL);

	// begin of header
	std::cout 
	<< "event.header {\n" 
	<< "  .fixed_header {\n"
	<< "    .event_type {\n"
	<< "      .domain_name = '"
	     << event.header.fixed_header.event_type.domain_name.in() << "'\n"
	<< "      .type_name = '" << event.header.fixed_header.event_type.type_name.in() << "'\n"
	<< "    }\n"
	<< "    .event_name = '" 
	        << event.header.fixed_header.event_name.in() << "'\n"
	<< "  }\n"
	<< "  .variable_header {" << std::endl;

	for(unsigned long i=0;i<event.header.variable_header.length();i++) {
		event.header.variable_header[i].value >>= extr;
		std::cout << "    [" << i << "] { " << ".name='" 
		<< event.header.variable_header[i].name.in() << "', "
		<< ".value='" << ptr << "' }" << std::endl;
	}
	
	std::cout 
	<< "  }\n" << "}" << std::endl;
	// end of header

	// begin of filterable_data
	std::cout << "  .filterable_data {" << std::endl;
	for(unsigned long i=0;i<event.filterable_data.length();i++) {
		event.filterable_data[i].value >>= extr;

		std::cout << "    [" << i << "] { "<< ".name='" 
		<< event.filterable_data[i].name.in() << "', "
		<< ".value='" << ptr << "' }" << std::endl;
	}
	std::cout 
	<< "  }\n" << "}" << std::endl;
	// end of filterable_data

	// remainder_of_body
	event.remainder_of_body >>= extr;
	std::cout << ".remainder_of_body = '" << ptr << "'" << std::endl;
}

static void print_event(const CORBA::Any& event)
{
	const char* ptr;
	CORBA::Any::to_string   extr(ptr, 0UL);

	event >>= extr;

	printf("%s\n", ptr);
}
