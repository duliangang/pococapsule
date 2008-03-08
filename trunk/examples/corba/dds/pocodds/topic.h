#ifndef _pocodds_topic_h
# define _pocodds_topic_h

#include "ddshelper.h"
#include "unsupported.h"

//
// implementation of topic
//
class POCO_DDS_Topic : public DDS::Topic {
	DDS::DomainParticipant_ptr      _my_participant;
	CORBA::String_var	_type_name;
	CORBA::String_var	_topic_name;

  public:
	POCO_DDS_Topic(	DDS::DomainParticipant_ptr part,
			const char* name,
			const char* type) 
		: _my_participant(part), _type_name(type), _topic_name(name) {}

	//
	// Topic description methods
	//
	char* get_type_name() { return CORBA::string_dup(_type_name.in()); }
	char* get_name() { return CORBA::string_dup(_topic_name.in()); }
	DDS::DomainParticipant_ptr get_participant() { 
		return DDS::DomainParticipant::_duplicate(_my_participant); }

	//
	// UNSUPPORTED
	//
	CORBA::Long set_qos(const DDS::TopicQos& _qos) UNSUPPORTED;
	CORBA::Long get_qos(DDS::TopicQos& _qos) UNSUPPORTED;
	CORBA::Long set_listener(DDS::TopicListener_ptr _a_listener,
		CORBA::ULong _mask) UNSUPPORTED;
	DDS::TopicListener_ptr get_listener() { return NULL; }
	CORBA::Long get_inconsistent_topic_status(
		DDS::InconsistentTopicStatus& _a_status) UNSUPPORTED;

	POCO_DDS_ENTITY_IMPL_METHODS
};

#endif
