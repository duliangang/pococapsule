#ifndef _pocodds_factory_h
# define _pocodds_factory_h

#include <map>

#include "ddshelper.h"
#include "unsupported.h"

typedef std::map<CORBA::Long, DDS::DomainParticipant_var> ParticipantMap;

//
// the domain participant factory
//
class POCO_DDS_DomainParticipantFactory : public DDS::DomainParticipantFactory {
	ParticipantMap	_participant_map;
	CORBA::ORB_ptr	_orb;
	CORBA::Object_var	_service;
	
  public:
	DDS::DomainParticipant_ptr create_participant(CORBA::Long _domain_id,
						const DDS::DomainParticipantQos& _qos,
						DDS::DomainParticipantListener_ptr _a_listener,
						CORBA::ULong _mask);
	CORBA::Long delete_participant(DDS::DomainParticipant_ptr _a_participant);
	DDS::DomainParticipant_ptr lookup_participant(CORBA::Long _domain_id);

	//
	// method not implemented for simplicity
	//
	CORBA::Long set_default_participant_qos(const DDS::DomainParticipantQos& _qos) UNSUPPORTED;
	CORBA::Long get_default_participant_qos(DDS::DomainParticipantQos& _qos) UNSUPPORTED;
	CORBA::Long set_qos(const DDS::DomainParticipantFactoryQos& _qos) UNSUPPORTED;
	CORBA::Long get_qos(DDS::DomainParticipantFactoryQos& _qos) UNSUPPORTED;
};

#endif
