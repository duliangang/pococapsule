#ifndef _poco_dds_unsupport_h
# define _poco_dds_unsupport_h

#define UNSUPPORTED { return DDS::RETCODE_UNSUPPORTED; }

//
// unsupported DDS::Entity methods
//
#define POCO_DDS_ENTITY_IMPL_METHODS \
	CORBA::Long enable() UNSUPPORTED; \
	DDS::StatusCondition_ptr get_statuscondition() { return NULL; } \
	CORBA::ULong get_status_changes() UNSUPPORTED; \
	CORBA::Long get_instance_handle() { return 0; }

//
// unsupported DDS::DataReader methods
//
#define POCO_DDS_DATAREADER_IMPL_METHODS \
	DDS::ReadCondition_ptr create_readcondition(CORBA::ULong, CORBA::ULong, CORBA::ULong) { return NULL; } \
	DDS::QueryCondition_ptr create_querycondition(CORBA::ULong _sample_states, CORBA::ULong _view_states, \
			CORBA::ULong _instance_states, const char* _query_expression, \
			const DDS::StringSeq& _query_parameters) { return NULL; } \
	CORBA::Long delete_readcondition(DDS::ReadCondition_ptr _a_condition) UNSUPPORTED; \
	CORBA::Long set_qos(const DDS::DataReaderQos& _qos) UNSUPPORTED; \
	CORBA::Long get_qos(DDS::DataReaderQos& _qos) UNSUPPORTED; \
	CORBA::Long set_listener(DDS::DataReaderListener_ptr _a_listener, \
                                       CORBA::ULong _mask) UNSUPPORTED; \
	DDS::DataReaderListener_ptr get_listener() { return NULL; } \
	DDS::TopicDescription_ptr get_topicdescription() { return NULL; } \
	DDS::Subscriber_ptr get_subscriber() { return NULL; } \
	CORBA::Long get_sample_rejected_status(DDS::SampleRejectedStatus& _status) UNSUPPORTED; \
	CORBA::Long get_liveliness_changed_status(DDS::LivelinessChangedStatus& _status) UNSUPPORTED; \
	CORBA::Long get_requested_deadline_missed_status(DDS::RequestedDeadlineMissedStatus& _status) UNSUPPORTED; \
	CORBA::Long get_requested_incompatible_qos_status(DDS::RequestedIncompatibleQosStatus& _status) UNSUPPORTED; \
	CORBA::Long get_subscription_matched_status(DDS::SubscriptionMatchedStatus& _status) UNSUPPORTED; \
	CORBA::Long get_sample_lost_status(DDS::SampleLostStatus& _status) UNSUPPORTED; \
	CORBA::Long wait_for_historical_data(const DDS::Duration_t& _max_wait) UNSUPPORTED; \
	CORBA::Long get_matched_publications(DDS::InstanceHandleSeq& _publication_handles) UNSUPPORTED; \
	CORBA::Long get_matched_publication_data(DDS::PublicationBuiltinTopicData& _publication_data, \
                                                       CORBA::Long _publication_handle) UNSUPPORTED;

//
// unsupported DDS::DataWriter methods
//
#define POCO_DDS_DATAWRITER_IMPL_METHODS \
	CORBA::Long set_qos(const DDS::DataWriterQos& _qos) UNSUPPORTED; \
	CORBA::Long get_qos(DDS::DataWriterQos& _qos) UNSUPPORTED; \
	CORBA::Long set_listener(DDS::DataWriterListener_ptr _a_listener, CORBA::ULong _mask) UNSUPPORTED; \
	DDS::DataWriterListener_ptr get_listener() { return NULL; } \
	DDS::Topic_ptr get_topic() { return NULL; } \
	DDS::Publisher_ptr get_publisher() { return NULL; } \
	CORBA::Long wait_for_acknowledgments(const DDS::Duration_t& _max_wait) UNSUPPORTED; \
	CORBA::Long get_liveliness_lost_status(DDS::LivelinessLostStatus& _liveliness_lost_status) UNSUPPORTED; \
	CORBA::Long get_offered_deadline_missed_status(DDS::OfferedDeadlineMissedStatus& _status) UNSUPPORTED; \
	CORBA::Long get_offered_incompatible_qos_status(DDS::OfferedIncompatibleQosStatus& _status) UNSUPPORTED; \
	CORBA::Long get_publication_matched_status(DDS::PublicationMatchedStatus& _status) UNSUPPORTED; \
	CORBA::Long assert_liveliness() UNSUPPORTED; \
	CORBA::Long get_matched_subscriptions(DDS::InstanceHandleSeq& _subscription_handles) UNSUPPORTED; \
	CORBA::Long get_matched_subscription_data(DDS::SubscriptionBuiltinTopicData& _subscription_data, \
                                                        CORBA::Long _subscription_handle) UNSUPPORTED; 

#endif
