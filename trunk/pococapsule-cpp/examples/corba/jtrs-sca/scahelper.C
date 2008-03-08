/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "scahelper.h"
#include <string>
#include <map>

#include "CF_c.hh"

//
// Mock the naming service
//
typedef std::map<std::string, CORBA::Object_ptr> ObjectMap;
static ObjectMap 	ns_map;

CORBA::Object_ptr poco_scahelper::findby_ns(const char* nm)
{
	std::string key(nm);
	ObjectMap::iterator itr = ns_map.find(key);

	if( itr == ns_map.end() ) {
		printf("[E6] Fail to resolve object with name %s\n", nm);
		return NULL;
	}

	return CORBA::Object::_duplicate(ns_map[key]);
}

CORBA::Object_ptr poco_scahelper::findby_ds(const char* name, const char* type)
{
	if( type != NULL && strcmp(type, "log") != 0 ) {
		return NULL;
	}

	return findby_ns("LogService");
}

void poco_scahelper::bindto_ns(const char* nm, CORBA::Object_ptr obj)
{
	std::string key(nm);
	ns_map[key] = CORBA::Object::_duplicate(obj);
}

void poco_scahelper::connect_interface(
			const char* uses_id, CORBA::Object_ptr user,
			const char* provides_id, CORBA::Object_ptr provider, const char* conn_id)
{
	CF::Resource_var ures = CF::Resource::_narrow(user);

	if( ures.in() == NULL ) {
		printf("[E1] Fail to wire up the connection %s: user is not a CF::Resource\n", conn_id);
		return;
	}

	CORBA::Object_var ref = ures->getPort(uses_id);

	if( ref.in() == NULL ) {
		printf("[E2] Fail to wire up the connection %s: fail to get port %s from the user object\n", conn_id, uses_id);
		return;
	}

	CF::Port_var port = CF::Port::_narrow(ref.in());

	if( port.in() == NULL ) {
		printf("[E3] Fail to wire up the connection %s: the port %s is not a CF::Port\n", conn_id, uses_id);
		return;
	}

	if( provides_id != NULL ) {
		CF::Resource_var pres = CF::Resource::_narrow(provider);
		if( pres.in() == NULL ) {
			printf("[E4] Fail to wire up the connection %s: the provider is not a CF::Resource\n", conn_id);
			return;
		}

		ref = pres->getPort(provides_id);

		if( ref.in() == NULL ) {
			printf("[E5] Fail to wire up the connection %s: fail to get port %s from the provider object\n", conn_id, provides_id);
			return;
		}

		provider = ref._retn();
	}

	port->connectPort(provider, conn_id);
}
