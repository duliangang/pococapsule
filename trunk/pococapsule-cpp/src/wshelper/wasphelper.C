/****************************************************************************/
/*                                                                          */
/*  Copyright 2006,2007 by Pocomatic Software, LLC. All Rights Reserved.    */
/*                                                                          */
/*  This program is free software: you can redistribute it and/or modify    */
/*  it under the terms of the GNU General Public License as published by    */
/*  the Free Software Foundation, either version 3 of the License, or       */
/*  (at your option) any later version.                                     */
/*                                                                          */
/*  This program is distributed in the hope that it will be useful,         */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           */
/*  GNU General Public License for more details.                            */
/*                                                                          */
/*  You should have received a copy of the GNU General Public License       */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.   */
/*                                                                          */
/*  Author: Ke Jin <kejin@pocomatic.com>				    */
/*									    */
/****************************************************************************/

#include <waspc/common.h>
#include <waspc/runtime/Runtime.h>
#include <waspc/stl/iooperators.h>
#include <waspc-impl/dispatcher/ServiceEndpointRepository.h>
#include <waspc-impl/service/CppServiceContextProperties.h>

#include "wshelper.h"

static int init_count = 0;
static int client_init_count = 0;

class POCO_WSClient {
  public:
};

class POCO_WSServer : public POCO_WSClient {
  public:
};

#include <pocoapp.h>

POCO_WSServer*	POCO_WSHelper::WS_init(const char* configFileName)
{
	static POCO_WSServer instance;

	init_count++;

	try {
		if( init_count == 1 ) {
			WASP_Runtime::serverInitialize();
		}

		if( configFileName != NULL ) {
			WASP_Runtime::load(configFileName, NULL);
		}

		if( init_count == 1 ) {
			WASP_Runtime::start(NULL);
		}
	}
	catch(WASP_Exception* e) {
		POCO_AppEnv* env = POCO_AppContext::getDefaultAppEnv();
		env->fatal_error(e->getCharMessage());
		delete e;
		return NULL;
	}

	return &instance;
}

void POCO_WSHelper::WS_run()
{
	if( init_count == 0 ) {
		return;
	}

	WASP_Runtime::waitForShutdown();
}

void POCO_WSHelper::WS_term(POCO_WSServer* server)
{
	init_count--;

	try {
		if( init_count == 0 ) {
			WASP_Runtime::stop();
			WASP_Runtime::serverTerminate();
		}
	}
	catch(WASP_Exception* e) {
		POCO_AppEnv* env = POCO_AppContext::getDefaultAppEnv();
		env->fatal_error(e->getCharMessage());

		delete e;
	}
}

void POCO_WSHelper::export_service(
			POCO_WSServer*		server,
			POCO_WSSkel*		serviceImpl,
			const char*		serviceName,
			const char*		wsdlFileName,
			const char*		dispatcherName)
{
	if( init_count == 0 || serviceImpl == NULL ) {
		return;
	}

	try {
		if( wsdlFileName ) {
			serviceImpl->service.setWSDL(WASP_VString(), wsdlFileName, true);
		}

		WASP_ServiceEndpoint endpoint(
				WASP_CppServiceContextProperties::createCppEndpoint(NULL));
		WASP_Context endpointContext(endpoint.getContext());
		WASP_CppServiceContextProperties::addServiceInstance(
				endpointContext, serviceImpl, -1, NULL);
		endpoint.start(NULL);

		WASP_ServiceEndpointRepository *endptrTable
				= WASP_ServiceEndpointRepository::getInstance();

		if( dispatcherName == NULL ) {
			dispatcherName = "DefaultDispatcher";
		}
		endptrTable->addEndpoint(dispatcherName, serviceName, endpoint, NULL);
	}
	catch(WASP_Exception* e) {
		POCO_AppEnv* env = POCO_AppContext::getDefaultAppEnv();
		env->fatal_error(e->getCharMessage());

		delete e;
	}
}

void POCO_WSHelper::binding(
			POCO_WSClient*			client,
			POCO_WSStub*			stub,
			const char*			url)
{
	if( stub == NULL || url == NULL ) {
		return;
	}

	stub->stub.setAddress(url, NULL);
}

POCO_WSClient*	POCO_WSHelper::WS_client_init(const char* configFileName)
{
	static POCO_WSClient instance;

	client_init_count++;

	try {
		if( client_init_count == 1 ) {
			WASP_Runtime::clientInitialize();
		}

		if( client_init_count == 1 ) {
			if( configFileName == NULL || *configFileName == 0 ) {
				configFileName = "conf/client-core.xml";
			}

			if( init_count == 0 ) {
				WASP_Runtime::clientStart(configFileName, NULL);
			}
		}
	}
	catch(WASP_Exception* e) {
		POCO_AppEnv* env = POCO_AppContext::getDefaultAppEnv();
		env->fatal_error(e->getCharMessage());
		delete e;
		return NULL;
	}

	return &instance;
}

void POCO_WSHelper::WS_client_term(POCO_WSClient* client)
{
	client_init_count--;

	try {
		if( init_count == 0 ) {
			WASP_Runtime::clientTerminate();
		}
	}
	catch(WASP_Exception* e) {
		POCO_AppEnv* env = POCO_AppContext::getDefaultAppEnv();
		env->fatal_error(e->getCharMessage());

		delete e;
	}
}
