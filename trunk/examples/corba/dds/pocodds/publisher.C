#include "publisher.h"
#include "typesupport.h"
#include "ddsproxy.h"
#include "corbahelper.h"

#include <iostream>
//
// writer proxy
//
class POCO_DDS_WriterProxy {
	CosNotifyComm::StructuredPushConsumer_var _consumer;
	CosNotification::StructuredEvent	  _event;
  public:
	POCO_DDS_WriterProxy(const char* domain_name, const char* topic_name, 
			CORBA::Object_ptr ref) {
		_event.header.fixed_header.event_type.domain_name = domain_name;
		_event.header.fixed_header.event_type.type_name  = "poco_dds";
		_event.header.fixed_header.event_name = topic_name;
		_consumer = CosNotifyComm::StructuredPushConsumer::_narrow(ref);
	}

	RetCode _push(const CORBA::Any& data) {
		_event.remainder_of_body = data;
		_consumer->push_structured_event(_event);
		//_consumer->_non_existent();

		return DDS::RETCODE_OK;
	}

	void	_flush() { if(_consumer) _consumer->_non_existent(); }
};

//
// data writer impl
//
POCO_DDS_DataWriterInternal::POCO_DDS_DataWriterInternal()
{
	_wproxy = NULL;
}

POCO_DDS_DataWriterInternal::~POCO_DDS_DataWriterInternal()
{
	delete _wproxy;
	_wproxy = NULL;
}

RetCode POCO_DDS_DataWriterInternal::_enqueue(const POCO_DDS_InputBuffer& buf)
{
	if( _wproxy ) {
		return _wproxy->_push(buf);
	}

	return DDS::RETCODE_ERROR;
}

//
// publisher
//
DDS::DataWriter_ptr POCO_DDS_Publisher::create_datawriter(
	DDS::Topic_ptr topic, 
	const DDS::DataWriterQos& qos,
	DDS::DataWriterListener_ptr listener,
	CORBA::ULong mask)
{
	CORBA::String_var	type_name = topic->get_type_name();
	CORBA::String_var	topic_name = topic->get_name();

	POCO_DDS_TypeSupport* typesup = poco_dds_resolve_type(type_name.in(), _participant);

	if( typesup == NULL ) {
		return NULL;
	}

	// create a data writer
	DDS::DataWriter_ptr writer = typesup->_create_datawriter();
	POCO_DDS_DataWriterInternal* impl = dynamic_cast<POCO_DDS_DataWriterInternal*>(writer);
	if( impl == NULL ) {
		return NULL;
	}

	char	domain_name[256];
	sprintf(domain_name, "dds-domain-%d", _participant->get_domain_id());
	char	header[256];
	sprintf(header, "%s:%s", domain_name, topic_name.in());

	impl->_cos_proxy = POCO_CORBAHelper::publish(
		_orb, _service, NULL, NULL, "structured", header, 0, NULL, (CORBA::Object_ptr)NULL);

	if( impl->_cos_proxy.in() == NULL ) {
		return NULL;
	}

	impl->_wproxy = new POCO_DDS_WriterProxy(domain_name, topic_name.in(), impl->_cos_proxy.in());

	return writer;
}

RetCode POCO_DDS_Publisher::delete_datawriter(DDS::DataWriter_ptr writer)
{
	POCO_DDS_DataWriterInternal* impl = dynamic_cast<POCO_DDS_DataWriterInternal*>(writer);

	if( impl != NULL ) {
		impl->_wproxy->_flush();
		POCO_CORBAHelper::unpublish(impl->_cos_proxy.in());
	}

	CORBA::release(writer);

	return DDS::RETCODE_OK;
}

_POCO_DDS_EXPORT const DDS::PublisherQos& poco_dds_get_publisher_qos_default()
{
	static DDS::PublisherQos	_instance;
	return _instance;
}

_POCO_DDS_EXPORT const DDS::DataWriterQos& poco_dds_get_datawriter_qos_default()
{
	static DDS::DataWriterQos	_instance;
	return _instance;
}
