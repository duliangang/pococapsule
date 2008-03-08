/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "sampleImpl.h"

void PortImpl::connectPort(CORBA::Object_ptr provider, const char* connId)
{
	static int count = 0;

	CF::Base_var obj = CF::Base::_narrow(provider);
	if( obj.in() == NULL ) {
		printf("[E7] provider for connection %s is not a CF::Base\n", connId);
		return;
	}

	CORBA::String_var name = obj->name();
	printf("[wire-%02d]: The %s is connected to the %s (connection-id: %s)\n", 
		count++, _name.in(), name.in(), connId); 
}

void PortImpl::disconnectPort(const char* connId)
{
	printf("Disconnected %s\n", connId);
}

void ResourceImpl::add_usesport(std::string name)
{
	std::string full_name = name + " port of the " + _name.in();

	_ports[name] = (new PortImpl(full_name.c_str()))->_this(); 
}

void ResourceImpl::add_providesport(std::string name)
{
	std::string full_name = name + " port of the " + _name.in();

	_ports[name] = (new PushPortImpl(full_name.c_str()))->_this();
}

CORBA::Object_ptr ResourceImpl::getPort(const char* port) {
	std::string key(port);
	ObjectMapItr	itr = _ports.find(key);
	if( itr == _ports.end() ) {
		return NULL;
	}

	return CORBA::Object::_duplicate(_ports[port]);
}
