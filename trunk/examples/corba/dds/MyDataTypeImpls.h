#ifndef _datatype_impls_h
# define _datatype_impls_h

#include "MyDataTypeDefs_c.hh"
#include "typesupport.h"

//
// To declare DDS type implementations
//
#ifdef USE_POCO_DDS
DDS_TYPE(Foo)
DDS_TYPE(Bar)
#endif

#endif
