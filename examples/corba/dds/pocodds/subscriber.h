#ifndef _poco_dds_subscriber_h
# define _poco_dds_subscriber_h

#include "ddshelper.h"
#include "unsupported.h"

#if defined(USE_TAO)
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosTypedEventCommC.h"
#endif

#if defined(USE_VBROKER)
#include "visinotify/CosNotifyComm_s.hh"
#include "visinotify/CosTypedEventComm_c.hh"
#endif

class POCO_DDS_Subscriber : public DDS::Subscriber {
	DDS::DomainParticipant_ptr	_participant;
	CORBA::ORB_ptr			_orb;
	CORBA::Object_ptr		_service;

  public:
	POCO_DDS_Subscriber(DDS::DomainParticipant_ptr part,
			CORBA::ORB_ptr orb, CORBA::Object_ptr svc) 
			: _participant(part), _orb(orb), _service(svc) {}

	DDS::DataReader_ptr create_datareader(
			DDS::TopicDescription_ptr _a_topic,
			const DDS::DataReaderQos& _qos,
			DDS::DataReaderListener_ptr _a_listener,
			CORBA::ULong _mask);

	CORBA::Long delete_datareader(DDS::DataReader_ptr);
	CORBA::Long delete_contained_entities() { return DDS::RETCODE_OK; }

	//
	// unsupported methods for simplicity
	//
	DDS::DataReader_ptr lookup_datareader(const char* _topic_name) { return NULL; }
	CORBA::Long set_qos(const DDS::SubscriberQos& _qos) UNSUPPORTED;
	CORBA::Long get_qos(DDS::SubscriberQos& _qos) UNSUPPORTED;
	CORBA::Long set_listener(DDS::SubscriberListener_ptr _a_listener,
				CORBA::ULong _mask) UNSUPPORTED;
	DDS::SubscriberListener_ptr get_listener() { return NULL; }
	CORBA::Long begin_coherent_changes() UNSUPPORTED;
	CORBA::Long end_coherent_changes() UNSUPPORTED;
	CORBA::Long wait_for_acknowledgments(const DDS::Duration_t& _max_wait) UNSUPPORTED;
	DDS::DomainParticipant_ptr get_participant() { return NULL; }
	CORBA::Long set_default_datareader_qos(const DDS::DataReaderQos& _qos) UNSUPPORTED;
	CORBA::Long get_default_datareader_qos(DDS::DataReaderQos& _qos) UNSUPPORTED;
	CORBA::Long copy_from_topic_qos(DDS::DataReaderQos& _a_datareader_qos,
				const DDS::TopicQos& _a_topic_qos) UNSUPPORTED;
	CORBA::Long get_datareaders(DDS::DataReaderSeq&, CORBA::ULong, CORBA::ULong, CORBA::ULong) UNSUPPORTED;
	CORBA::Long notify_datareaders() UNSUPPORTED;
	CORBA::Long begin_access() UNSUPPORTED;
	CORBA::Long end_access() UNSUPPORTED;

	POCO_DDS_ENTITY_IMPL_METHODS
};

#endif
