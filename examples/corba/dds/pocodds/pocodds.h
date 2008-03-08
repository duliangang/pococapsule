#if !defined(__INCLUDE_POCODDS_H_)
#define __INCLUDE_POCODDS_H_

#ifndef _POCO_DDS_EXPORT 
#ifdef WIN32
# ifdef BUILD_POCO_DDS_DLL
#  define _POCO_DDS_EXPORT __declspec(dllexport)
# else
#  define _POCO_DDS_EXPORT __declspec(dllimport)
# endif
#else
# define _POCO_DDS_EXPORT
#endif
#endif
 
#if defined(USE_TAO)&& defined(WIN32)
namespace DDS {
class _POCO_DDS_EXPORT Entity;
class _POCO_DDS_EXPORT Topic;
class _POCO_DDS_EXPORT DataReader;
class _POCO_DDS_EXPORT DataWriter;
class _POCO_DDS_EXPORT Listener;
class _POCO_DDS_EXPORT DataReaderListener;
class _POCO_DDS_EXPORT DataWriterListener;
class _POCO_DDS_EXPORT Subscriber;
class _POCO_DDS_EXPORT Publisher;
class _POCO_DDS_EXPORT PublisherListener;
class _POCO_DDS_EXPORT SubscriberListener;
class _POCO_DDS_EXPORT TopicListener;
class _POCO_DDS_EXPORT TypeSupport;
class _POCO_DDS_EXPORT DomainParticipant;
class _POCO_DDS_EXPORT DomainParticipantListener;
class _POCO_DDS_EXPORT DomainParticipantFactory;
};

#include "tao/Objref_VarOut_T.h"

namespace TAO {

template<>
struct _POCO_DDS_EXPORT Objref_Traits<DDS::Listener>;

template<>
struct _POCO_DDS_EXPORT Objref_Traits<DDS::PublisherListener>;

template<>
struct _POCO_DDS_EXPORT Objref_Traits<DDS::TopicListener>;

template<>
struct _POCO_DDS_EXPORT Objref_Traits<DDS::DataWriterListener>;

template<>
struct _POCO_DDS_EXPORT Objref_Traits<DDS::SubscriberListener>;

template<>
struct _POCO_DDS_EXPORT Objref_Traits<DDS::DomainParticipantListener>;

template<>
struct _POCO_DDS_EXPORT Objref_Traits<DDS::DataReaderListener>;

};
#endif

#endif
