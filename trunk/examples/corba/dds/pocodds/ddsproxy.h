#ifndef _poco_proxy_h
# define _poco_proxy_h

#include "ddshelper.h"
#include "dds_dcps_c.hh"

//
// Simple implementation: using CORBA::Any as code/decode buffer
//
typedef CORBA::Any POCO_DDS_InputBuffer;
typedef CORBA::Any POCO_DDS_OutputBuffer;

#ifdef USE_TAO
#include "tao/AnyTypeCode/Any.h"
#define POCO_DDS_DEMARSHAL(buf, XYZ, val) { XYZ* u; (buf) >>= (u); val = *u; }
#endif

#ifdef USE_VBROKER
#define POCO_DDS_DEMARSHAL(buf, VAR, val) { (buf) >>= (val); }
#endif

#define POCO_DDS_MARSHAL(buf, val)   { (buf) <<= (val); }

typedef DDS::ReturnCode_t       RetCode;

class POCO_DDS_ReaderProxy;
class POCO_DDS_WriterProxy;

class _POCO_DDS_EXPORT POCO_DDS_DataReaderInternal {
  protected:
	CORBA::Object_var	_cos_proxy;
	POCO_DDS_ReaderProxy*	_rproxy;
  public:
	POCO_DDS_DataReaderInternal();
	virtual ~POCO_DDS_DataReaderInternal();
	RetCode _dequeue(POCO_DDS_InputBuffer&);
	friend class POCO_DDS_Subscriber;
};

class _POCO_DDS_EXPORT POCO_DDS_DataWriterInternal {
  protected:
	CORBA::Object_var	_cos_proxy;
	POCO_DDS_WriterProxy*	_wproxy;

  public:
	POCO_DDS_DataWriterInternal();
	virtual ~POCO_DDS_DataWriterInternal();
	RetCode _enqueue(const POCO_DDS_OutputBuffer&);
	friend class POCO_DDS_Publisher;
};

#endif
