#include "dds_dcps_c.hh"
#include "factory.h"
#include "participant.h"

#if defined(USE_TAO) && defined(WIN32)
namespace DDS {
class _POCO_DDS_EXPORT DataReader;
class _POCO_DDS_EXPORT Entity;
};
#endif
//
// implementation of the domain participant factory
//
DDS::DomainParticipant_ptr POCO_DDS_DomainParticipantFactory::create_participant(
	DDS::DomainId_t	domain_id,
        const DDS::DomainParticipantQos& qos,
        DDS::DomainParticipantListener_ptr listener,
        DDS::StatusMask mask)
{
	ParticipantMap::iterator itr = _participant_map.find(domain_id);

	if( _service.in() == NULL ) {
		int argc = 0;
		char* argv[] = { NULL };
		_orb = CORBA::ORB_init(argc, argv);
		_service = _orb->resolve_initial_references("NotificationService");

		if( _service.in() == NULL ) {
			printf("Fail to resolve the needed Notification Service\n");
			return NULL;
		}
	}

	DDS::DomainParticipant_var var;

	if( itr == _participant_map.end() ) {
		// qos, listener and mask are ignored for simplicity
		var = new POCO_DDS_DomainParticipant(domain_id, _orb, _service);
		_participant_map[domain_id] = var;
	}
	else {
		var = itr->second;
	}

	return var._retn();
}

CORBA::Long POCO_DDS_DomainParticipantFactory::delete_participant(
	DDS::DomainParticipant_ptr ptr)
{
	_participant_map.erase(ptr->get_domain_id());

	return DDS::RETCODE_OK;
}

DDS::DomainParticipant_ptr POCO_DDS_DomainParticipantFactory::lookup_participant(
	DDS::DomainId_t domain_id)
{
	ParticipantMap::iterator itr = _participant_map.find(domain_id);

	if( itr == _participant_map.end() ) {
		return NULL;
	}

	DDS::DomainParticipant_var var = itr->second;
	return var._retn();
}

//
// automatic registration of the domain factory, to have it resolveable as a rir
//
#ifdef USE_TAO
#include <tao/PI/ORBInitializerC.h>
#include <tao/PI/ORBInitInfo.h>
#include <tao/ORBInitializer_Registry.h>
#endif

class POCO_DDS_FactoryInitializer : public PortableInterceptor::ORBInitializer
{
  public:
	POCO_DDS_FactoryInitializer() {
		PortableInterceptor::register_orb_initializer(this);
	}

	void pre_init(PortableInterceptor::ORBInitInfo_ptr info) {
		info->register_initial_reference("DataDistributionService", new POCO_DDS_DomainParticipantFactory);
	}

	void post_init(PortableInterceptor::ORBInitInfo_ptr info) { }
};

static POCO_DDS_FactoryInitializer factory_initializer;
