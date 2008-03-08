/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _my_emitters_impl_h_
# define _my_emitters_impl_h_

#ifdef USE_VBROKER
#include <CosNotification_c.hh>
#include <CosTypedEventComm_c.hh>
#include <CosNotifyComm_c.hh>
#endif

#ifdef USE_TAO
#include <orbsvcs/CosNotificationC.h>
#include <orbsvcs/CosTypedEventCommC.h>
#include <orbsvcs/CosNotifyCommC.h>
#endif

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

//
// A native implementation provides signatures to inject all 
// types (typed, structured, sequence, or untyped) proxy consumers. 
//
// This implementation is not a POA servant, therefore, is going 
// to be deployed with kind="native".
//

class POCO_EXPORT EventEmitterImpl {
  public:
	//	
	// this operation is the port for container to inject
	// the connection to remote event sink proxy.
	//
	void connect_typed_push_consumer(
		CosTypedEventComm::TypedPushConsumer_ptr consumer);

	//
	// this operation is the port for container to inject	
	// the connection to remote event sink proxy.
	//
	void connect_structured_push_consumer(
		CosNotifyComm::StructuredPushConsumer_ptr consumer);

	//
	// this operation is the port for container to inject
	// the connection to remote event sink proxy.
	//
	void connect_sequence_push_consumer(
		CosNotifyComm::SequencePushConsumer_ptr consumer);

	//
	// this operation is the port for container to inject 
	// the connection to remote event sink proxy.
	//
	void connect_any_push_consumer(
		CosEventComm::PushConsumer_ptr consumer);
};

#endif
