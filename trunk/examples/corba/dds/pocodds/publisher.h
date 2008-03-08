#ifndef _poco_dds_publisher_h
# define _poco_dds_publisher_h

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

class POCO_DDS_Publisher : public DDS::Publisher {
	DDS::DomainParticipant_ptr	_participant;
	CORBA::ORB_ptr			_orb;
	CORBA::Object_ptr		_service;

  public:
	POCO_DDS_Publisher(DDS::DomainParticipant_ptr part,
			CORBA::ORB_ptr orb, CORBA::Object_ptr svc) 
			: _participant(part), _orb(orb), _service(svc) {}

	DDS::DataWriter_ptr create_datawriter(
			DDS::Topic_ptr _a_topic,
			const DDS::DataWriterQos& _qos,
			DDS::DataWriterListener_ptr _a_listener,
			CORBA::ULong _mask);

	CORBA::Long delete_datawriter(DDS::DataWriter_ptr _a_datawriter);
	CORBA::Long delete_contained_entities() { return DDS::RETCODE_OK; }

	//
	// unsupport methods for simplicity
	//
	DDS::DataWriter_ptr lookup_datawriter(const char* _topic_name) { return NULL; }
	CORBA::Long set_qos(const DDS::PublisherQos& _qos) UNSUPPORTED;
	CORBA::Long get_qos(DDS::PublisherQos& _qos) UNSUPPORTED;
	CORBA::Long set_listener(DDS::PublisherListener_ptr _a_listener,
                                       CORBA::ULong _mask) UNSUPPORTED;
	DDS::PublisherListener_ptr get_listener() { return NULL; }
	CORBA::Long suspend_publications() UNSUPPORTED;
	CORBA::Long resume_publications() UNSUPPORTED;
	CORBA::Long begin_coherent_changes() UNSUPPORTED;
	CORBA::Long end_coherent_changes() UNSUPPORTED;
	CORBA::Long wait_for_acknowledgments(const DDS::Duration_t& _max_wait) UNSUPPORTED;
	DDS::DomainParticipant_ptr get_participant() { return NULL; }
	CORBA::Long set_default_datawriter_qos(const DDS::DataWriterQos& _qos) UNSUPPORTED;
	CORBA::Long get_default_datawriter_qos(DDS::DataWriterQos& _qos) UNSUPPORTED;
	CORBA::Long copy_from_topic_qos(DDS::DataWriterQos& _a_datawriter_qos,
                                              const DDS::TopicQos& _a_topic_qos) UNSUPPORTED;

	POCO_DDS_ENTITY_IMPL_METHODS
};

#endif
