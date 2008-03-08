#ifndef _pocodds_participant_h
# define _pocodds_participant_h

#include <map>
#include <set>
#include <string>

#include "ddshelper.h"
#include "unsupported.h"
#include "typesupport.h"
#include "dds_dcps_c.hh"

typedef std::map<std::string, DDS::TypeSupport_var> TypeSupportMap;
typedef std::map<std::string, DDS::Topic_var> TopicMap;

//
// implementation of the domain participant and topic
//
class POCO_DDS_DomainParticipant : public DDS::DomainParticipant 
{
	CORBA::ULong	_domain_id;

	TypeSupportMap	_type_support_map;
	TopicMap	_topic_map;
	
	CORBA::Long	_register_type(POCO_DDS_TypeSupport* typesup);
	POCO_DDS_TypeSupport* _resolve_type(const char* name);

	CORBA::ORB_ptr	_orb;
	CORBA::Object_ptr _service;

  public:
	friend class POCO_DDS_Subscriber;
	friend class POCO_DDS_Publisher;
	friend RetCode poco_dds_register_type(POCO_DDS_TypeSupport*, DDS::DomainParticipant_ptr);
	friend POCO_DDS_TypeSupport* poco_dds_resolve_type(const char*, DDS::DomainParticipant_ptr);

	POCO_DDS_DomainParticipant(CORBA::ULong id, CORBA::ORB_ptr orb, CORBA::Object_ptr svc)
			: _domain_id(id), _orb(orb), _service(svc) {}

	DDS::Publisher_ptr create_publisher(
			const DDS::PublisherQos& _qos,
			DDS::PublisherListener_ptr _a_listener,
			CORBA::ULong _mask);
	CORBA::Long delete_publisher(DDS::Publisher_ptr _p) { return DDS::RETCODE_OK; }
	DDS::Subscriber_ptr create_subscriber(
			const DDS::SubscriberQos& _qos,
			DDS::SubscriberListener_ptr _a_listener,
			CORBA::ULong _mask);
	CORBA::Long delete_subscriber(DDS::Subscriber_ptr _s) { return DDS::RETCODE_OK; }

	DDS::Topic_ptr create_topic(
			const char* _topic_name,
			const char* _type_name,
			const DDS::TopicQos& _qos,
			DDS::TopicListener_ptr _a_listener,
			CORBA::ULong _mask);
	CORBA::Long delete_topic(DDS::Topic_ptr _a_topic) { return DDS::RETCODE_OK; }
        CORBA::Long delete_contained_entities() { return DDS::RETCODE_OK; }

	//
	// to be used by subscribers, publishers and readers/writers
	//
	DDS::Topic_ptr find_topic(const char* _topic_name, const DDS::Duration_t& _timeout);
	DDS::TopicDescription_ptr lookup_topicdescription(const char* _name);

	//
	// methods that are not implemented for simplicity.
	//
        DDS::Subscriber_ptr get_builtin_subscriber() { return NULL; }
	DDS::ContentFilteredTopic_ptr create_contentfilteredtopic(const char* _name,
			DDS::Topic_ptr _related_topic,
			const char* _filter_expression,
			const DDS::StringSeq& _expression_parameters) { return NULL; }
	CORBA::Long delete_contentfilteredtopic(
			DDS::ContentFilteredTopic_ptr _a_contentfilteredtopic) UNSUPPORTED;
	DDS::MultiTopic_ptr create_multitopic(const char* _name,
			const char* _type_name,
			const char* _subscription_expression,
			const DDS::StringSeq& _expression_parameters) { return NULL; }
	CORBA::Long delete_multitopic(DDS::MultiTopic_ptr _a_multitopic) UNSUPPORTED;
	CORBA::Long set_qos(const DDS::DomainParticipantQos& _qos) UNSUPPORTED;
	CORBA::Long get_qos(DDS::DomainParticipantQos& _qos) UNSUPPORTED;
	CORBA::Long set_listener(DDS::DomainParticipantListener_ptr _a_listener,
			CORBA::ULong _mask) UNSUPPORTED;
	DDS::DomainParticipantListener_ptr get_listener() { return NULL; }
	CORBA::Long ignore_participant(CORBA::Long _handle) UNSUPPORTED;
	CORBA::Long ignore_topic(CORBA::Long _handle) UNSUPPORTED;
	CORBA::Long ignore_publication(CORBA::Long _handle) UNSUPPORTED;
	CORBA::Long ignore_subscription(CORBA::Long _handle) UNSUPPORTED;
	CORBA::Long get_domain_id() { return _domain_id; }
	CORBA::Long assert_liveliness() UNSUPPORTED;
	CORBA::Long set_default_publisher_qos(const DDS::PublisherQos& _qos) UNSUPPORTED;
	CORBA::Long get_default_publisher_qos(DDS::PublisherQos& _qos) UNSUPPORTED;
	CORBA::Long set_default_subscriber_qos(const DDS::SubscriberQos& _qos) UNSUPPORTED;
	CORBA::Long get_default_subscriber_qos(DDS::SubscriberQos& _qos) UNSUPPORTED;
	CORBA::Long set_default_topic_qos(const DDS::TopicQos& _qos) UNSUPPORTED;
	CORBA::Long get_default_topic_qos(DDS::TopicQos& _qos) UNSUPPORTED;
	CORBA::Long get_discovered_participants(DDS::InstanceHandleSeq& _participant_handles) UNSUPPORTED;
	CORBA::Long get_discovered_participant_data(
			DDS::ParticipantBuiltinTopicData& _participant_data,
			CORBA::Long _participant_handle) UNSUPPORTED;
	CORBA::Long get_discovered_topics(DDS::InstanceHandleSeq& _topic_handles) UNSUPPORTED;
	CORBA::Long get_discovered_topic_data(DDS::TopicBuiltinTopicData& _topic_data,
			CORBA::Long _topic_handle) UNSUPPORTED;
	CORBA::Boolean contains_entity(CORBA::Long _a_handle) UNSUPPORTED;
	CORBA::Long get_current_time(DDS::Time_t& _current_time) UNSUPPORTED;

	POCO_DDS_ENTITY_IMPL_METHODS
};

#endif
