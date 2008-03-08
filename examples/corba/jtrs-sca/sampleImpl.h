/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _sample_impl_h
#define _sample_impl_h

#include "sample_s.hh"
#include "pocodefs.h" // for the macro POCO_EXPORT -- to export on Windows

class POCO_EXPORT PushPortImpl 
	: public POA_sample::PushPort, public PortableServer::RefCountServantBase 
{
	CORBA::String_var	_name;
  public:
	PushPortImpl(const char* name) : _name(name) {}
	char*	name() { return CORBA::string_dup(_name.in()); }
};

class POCO_EXPORT LogImpl : public POA_sample::Log, public PortableServer::RefCountServantBase 
{
  public:
	char*   name() { return CORBA::string_dup("log"); }
};

class POCO_EXPORT PortImpl
	: public POA_CF::Port, public virtual PortableServer::RefCountServantBase
{
	CORBA::String_var	_name;

  public:
	PortImpl(const char* name) : _name(name) {}
	char* name() { return CORBA::string_dup(_name.in()); }

	void connectPort(CORBA::Object_ptr provider, const char* connId);
	void disconnectPort(const char* connId);
};

#include <string>
#include <map>

typedef std::map<std::string, CORBA::Object_ptr> ObjectMap;
typedef ObjectMap::iterator                      ObjectMapItr;

class POCO_EXPORT ResourceImpl 
	: public POA_CF::Resource, public virtual PortableServer::RefCountServantBase
{
	CORBA::String_var	_name;
	ObjectMap		_ports; 

  public:
	ResourceImpl(const char* name) : _name(name) {}
	char* name() { return CORBA::string_dup(_name.in()); }

	void add_usesport(std::string name);
	void add_providesport(std::string name);

	CORBA::Object_ptr getPort(const char* port);
};

#endif
