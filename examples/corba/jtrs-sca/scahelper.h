/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef poco_jtrs_scahelper_h
# define poco_jtrs_scahelper_h

#ifdef USE_VBROKER
#include <corba.h>
#endif

#ifdef USE_TAO
#include <tao/corba.h>
#endif

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

class POCO_EXPORT poco_scahelper {
  public:
	static CORBA::Object_ptr findby_ns(const char* nm);
	static void bindto_ns(const char* nm, CORBA::Object_ptr obj);

	static CORBA::Object_ptr findby_ds(const char* name, const char* type);

	static void connect_interface(const char* uses_id, CORBA::Object_ptr user,
				      const char* provides_id, CORBA::Object_ptr provider, const char* conn_id);
};

#endif
