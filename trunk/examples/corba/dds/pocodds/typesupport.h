#ifndef _sample_type_support
# define _sample_type_support

#include "unsupported.h"
#include "ddsproxy.h"
//
// base class for type support impl.
//
class POCO_DDS_TypeSupport : public DDS::TypeSupport {
  public:
	virtual RetCode register_type(DDS::DomainParticipant_ptr, const char*) = 0;
	virtual char* get_type_name() = 0;

	friend class POCO_DDS_Subscriber;
	friend class POCO_DDS_Publisher;

  protected:
	virtual DDS::DataReader_ptr _create_datareader() = 0;
	virtual DDS::DataWriter_ptr _create_datawriter() = 0;
};

//
// export and import types
//
_POCO_DDS_EXPORT RetCode 
	poco_dds_register_type(POCO_DDS_TypeSupport*, DDS::DomainParticipant_ptr);
POCO_DDS_TypeSupport* poco_dds_resolve_type(const char* name, DDS::DomainParticipant_ptr);

template <class XYZ, class XYZDataReader>
class POCO_DDS_DataReaderImpl : public XYZDataReader, public POCO_DDS_DataReaderInternal {
  public:
	friend class POCO_DDS_Subscriber;

	CORBA::Long take_next_sample(XYZ& v, DDS::SampleInfo& info) {
		POCO_DDS_InputBuffer buf;
		RetCode ret = _dequeue(buf);
		if( ret == DDS::RETCODE_OK ) {
			POCO_DDS_DEMARSHAL(buf, XYZ, v);
		}
		return ret;
	}

	CORBA::Long delete_contained_entities() { return DDS::RETCODE_OK; }

	POCO_DDS_DATAREADER_IMPL_METHODS
	POCO_DDS_ENTITY_IMPL_METHODS
};

template <class XYZ, class XYZDataWriter>
class POCO_DDS_DataWriterImpl : public XYZDataWriter, public POCO_DDS_DataWriterInternal {
  public:
	friend class POCO_DDS_Publisher;

	CORBA::Long write(const XYZ& val, DDS::InstanceHandle_t handle) {
		POCO_DDS_OutputBuffer buf;
		POCO_DDS_MARSHAL(buf, val);
		return _enqueue(buf);
	}

	POCO_DDS_DATAWRITER_IMPL_METHODS
	POCO_DDS_ENTITY_IMPL_METHODS
};

#define DDS_TYPE(XYZ) \
class XYZ##TypeSupportImpl : public POCO_DDS_TypeSupport { \
	CORBA::String_var	_name; \
\
	DDS::DataReader_ptr _create_datareader() { \
		return new POCO_DDS_DataReaderImpl<XYZ, XYZ##DataReader>; \
	} \
	DDS::DataWriter_ptr _create_datawriter() { \
		return new POCO_DDS_DataWriterImpl<XYZ, XYZ##DataWriter>; \
	} \
\
  public: \
	friend class POCO_DDS_Subscriber; \
	friend class POCO_DDS_Publisher; \
\
	XYZ##TypeSupportImpl() : _name(#XYZ) {} \
\
	RetCode register_type(DDS::DomainParticipant_ptr p, const char* name) { \
		if( name != NULL && name[0] != 0) { \
			_name = name; \
		} \
\
		return poco_dds_register_type(this, p); \
	} \
\
	char* get_type_name() { return CORBA::string_dup(_name.in()); }; \
}; 

#endif
