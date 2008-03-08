#include "participant.h"
#include "topic.h"
#include "publisher.h"
#include "subscriber.h"

//
// implementation of the domain participant 
//
DDS::Publisher_ptr POCO_DDS_DomainParticipant::create_publisher(
		const DDS::PublisherQos& _qos,
		DDS::PublisherListener_ptr _a_listener,
		CORBA::ULong _mask)
{
	return new POCO_DDS_Publisher(this, _orb, _service);
}

DDS::Subscriber_ptr POCO_DDS_DomainParticipant::create_subscriber(
		const DDS::SubscriberQos& _qos,
		DDS::SubscriberListener_ptr _a_listener,
		CORBA::ULong _mask)
{
        return new POCO_DDS_Subscriber(this, _orb, _service);
}

DDS::Topic_ptr POCO_DDS_DomainParticipant::create_topic(
		const char*	topic_name,
		const char*	type_name,
		const DDS::TopicQos&	_qos,
		DDS::TopicListener_ptr _a_listener,
		CORBA::ULong	_mask)
{
	TypeSupportMap::iterator type_itr = _type_support_map.find(type_name);

	if( type_itr == _type_support_map.end() ) {
		// typesupport not registered yet.
		return NULL;
	}

	TopicMap::iterator topic_itr = _topic_map.find(topic_name);
	
	DDS::Topic_var var;
	if( topic_itr == _topic_map.end() ) {
		var = new POCO_DDS_Topic(this, topic_name, type_name);
	}
	else {
		CORBA::String_var type_name_var = topic_itr->second->get_type_name();

		if( strcmp(type_name_var.in(), type_name) ) {
			// 
			// type name fail to match with the exiting topic. 
			//
			return NULL;
		}
		else {
			//
			// a duplicated topic 
			// (we don't keep track count of duplications for simplicity)
			//
			var = topic_itr->second;
		}
	}

	return var._retn();
}

DDS::Topic_ptr POCO_DDS_DomainParticipant::find_topic(
		const char* name, const DDS::Duration_t& timeout)
{
	TopicMap::iterator itr = _topic_map.find(name);

	if( itr == _topic_map.end() ) {
		return NULL;
	}

	DDS::Topic_var var = itr->second;

	return var._retn();
}

DDS::TopicDescription_ptr POCO_DDS_DomainParticipant::lookup_topicdescription(
		const char* name)
{
	DDS::Duration_t timeout;
	return find_topic(name, timeout);
}

RetCode POCO_DDS_DomainParticipant::_register_type(POCO_DDS_TypeSupport* typesup)
{
	CORBA::String_var type_name = typesup->get_type_name();
	TypeSupportMap::iterator itr = _type_support_map.find(type_name.in());

	if( itr != _type_support_map.end() 
	 && itr->second.in() != (DDS::TypeSupport_ptr)typesup ) {
		// ignore more error check for simplicity
		return DDS::RETCODE_OK;
	}

	DDS::TypeSupport_var tvar(DDS::TypeSupport::_duplicate(typesup));

	_type_support_map[typesup->get_type_name()] = tvar;

	return DDS::RETCODE_OK;
}

POCO_DDS_TypeSupport* POCO_DDS_DomainParticipant::_resolve_type(const char* name)
{
	TypeSupportMap::iterator itr = _type_support_map.find(name);

	if( itr == _type_support_map.end() ) {
		return NULL;
	}

	//
	// note: the return value is not ref-count'ed!!!
	//
	return dynamic_cast<POCO_DDS_TypeSupport*>(itr->second.in());
}

//
// method to be used by typesupport.
//
_POCO_DDS_EXPORT RetCode poco_dds_register_type(POCO_DDS_TypeSupport* typesup, DDS::DomainParticipant_ptr part)
{
	POCO_DDS_DomainParticipant* partImpl = dynamic_cast<POCO_DDS_DomainParticipant*>(part);

	if( partImpl == NULL ) {
		return DDS::RETCODE_BAD_PARAMETER;
	}

	return partImpl->_register_type(typesup);
}

POCO_DDS_TypeSupport* poco_dds_resolve_type(const char* name, DDS::DomainParticipant_ptr part)
{
	POCO_DDS_DomainParticipant* partImpl = dynamic_cast<POCO_DDS_DomainParticipant*>(part);

	if( partImpl == NULL ) {
		return NULL;
	}

	return partImpl->_resolve_type(name);
}

_POCO_DDS_EXPORT const DDS::DomainParticipantQos& poco_dds_get_participant_qos_default()
{
	static DDS::DomainParticipantQos _instance;
	return _instance;
}

_POCO_DDS_EXPORT const DDS::TopicQos&       poco_dds_get_topic_qos_default()
{
	static DDS::TopicQos _instance;
	return _instance;
}
