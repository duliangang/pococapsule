#include "subscriber.h"
#include "typesupport.h"
#include "ddsproxy.h"
#include "corbahelper.h"
#include <iostream>
//
// reader proxy
//
class POCO_DDS_ReaderProxy {
	CORBA::Any _data; // The queue is simplified to size 1,
			  // and assume no concurrent access.
	int	_count;   // 0 or 1 only.
  public:
	POCO_DDS_ReaderProxy() : _count(0) {}

	void _enqueue(const CORBA::Any& data) { _data = data; _count = 1; }
	RetCode _dequeue(CORBA::Any& buf) { 
		if( _count == 0 ) {
			return DDS::RETCODE_NO_DATA;
		}

		_count--;

		buf = _data;
		return DDS::RETCODE_OK;
	}
};

//
// data reader impl
//
POCO_DDS_DataReaderInternal::POCO_DDS_DataReaderInternal()
{
	_rproxy = new POCO_DDS_ReaderProxy;
}

POCO_DDS_DataReaderInternal::~POCO_DDS_DataReaderInternal()
{
	delete _rproxy;
}

RetCode POCO_DDS_DataReaderInternal::_dequeue(POCO_DDS_InputBuffer& buf)
{
	return _rproxy->_dequeue(buf);
}

//
// event listener wrapper
//
class EventListenerWrapper : 
		public POA_CosNotifyComm::StructuredPushConsumer,
		public virtual PortableServer::RefCountServantBase
 {
	DDS::DataReaderListener_var	_listener; // the app provided listener. 
	POCO_DDS_ReaderProxy*		_rproxy;   // the reader proxy
	DDS::DataReader_var		_reader;   // the reader, that contains the reader proxy.

  public:
	friend class POCO_DDS_Subscriber;

	EventListenerWrapper(DDS::DataReaderListener_ptr lsner, POCO_DDS_ReaderProxy* rproxy, DDS::DataReader_ptr reader ) { 
		// user provided listener
		_listener = DDS::DataReaderListener::_duplicate(lsner);
		_rproxy = rproxy;
		_reader = DDS::DataReader::_duplicate(reader);
	}

	void push_structured_event(
                const CosNotification::StructuredEvent& event) {
		_rproxy->_enqueue(event.remainder_of_body);
		_listener->on_data_available(_reader);
	}

	void disconnect_structured_push_consumer() {}
	void offer_change(
		const CosNotification::EventTypeSeq&,
		const CosNotification::EventTypeSeq&) {}
};

//
// subscriber
//
DDS::DataReader_ptr POCO_DDS_Subscriber::create_datareader(
	DDS::TopicDescription_ptr topic, 
	const DDS::DataReaderQos& qos,
	DDS::DataReaderListener_ptr listener,
	CORBA::ULong mask)
{
	if( listener == NULL ) {
		// 
		// for simplicity, reader listener is mandated.
		//
		return NULL;
	}

	CORBA::String_var	type_name = topic->get_type_name();
	CORBA::String_var	topic_name = topic->get_name();

	POCO_DDS_TypeSupport* typesup = poco_dds_resolve_type(type_name.in(), _participant);

	if( typesup == NULL ) {
		return NULL;
	}

	// create a data reader
	DDS::DataReader_ptr reader = typesup->_create_datareader();
	POCO_DDS_DataReaderInternal* impl = dynamic_cast<POCO_DDS_DataReaderInternal*>(reader);
	if( impl == NULL ) {
		return NULL;
	}

	// allocate a listener wrapper
	PortableServer::ServantBase_var wrapper 
		= new EventListenerWrapper(listener, impl->_rproxy, reader);

	char	filter[256];
	sprintf(filter, "($domain_name == 'dds-domain-%d') and ($event_name == '%s')", 
			_participant->get_domain_id(), topic_name.in());

	POCO_NameValue props[1] = {{"filter", filter}};

	impl->_cos_proxy = POCO_CORBAHelper::subscribe(
		_orb, _service, NULL, NULL, "structured", NULL, 1, props, wrapper.in());

	if( impl->_cos_proxy.in() == NULL ) {
		return NULL;
	}

	return reader;
}

RetCode POCO_DDS_Subscriber::delete_datareader(DDS::DataReader_ptr reader)
{
	POCO_DDS_DataReaderInternal* impl = dynamic_cast<POCO_DDS_DataReaderInternal*>(reader);

	if( impl != NULL ) {
		POCO_CORBAHelper::unsubscribe(impl->_cos_proxy.in());
	}

	CORBA::release(reader);

	return DDS::RETCODE_OK;
}

_POCO_DDS_EXPORT const DDS::SubscriberQos& poco_dds_get_subscriber_qos_default()
{
	static DDS::SubscriberQos _instance;
	return _instance;
}

_POCO_DDS_EXPORT const DDS::DataReaderQos&  poco_dds_get_datareader_qos_default()
{
	static DDS::DataReaderQos _instance;
	return _instance;
}
