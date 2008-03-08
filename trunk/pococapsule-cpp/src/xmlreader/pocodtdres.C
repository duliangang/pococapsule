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

#include <xercesc/framework/MemBufInputSource.hpp>
#include "pocodtdres.h"
#include <stdlib.h>
#include "pocostr.h"
#include "xmlreader.h"

InputSource* POCO_DtdResolver::resolveEntity(
	const XMLCh* const    publicId,
	const XMLCh* const    systemId)
{
	static const char* poco_dtd_id 
		= "http://www.pocomatic.com/poco-application-context.dtd";

	POCO_String sysid = POCO_XML_Reader::transcode(systemId);

	if( strcmp(sysid.in(), poco_dtd_id) ) {
		return NULL;
	}

	const XMLByte* dtd = (const XMLByte*)pocomatic_get_dtd();

	return new MemBufInputSource(dtd, strlen((const char*)dtd), "dtd-str");
}
