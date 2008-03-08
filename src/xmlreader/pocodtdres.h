/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/*  This code is the property of Pocomatic Software LLC., and is            */
/*  protected  by  copyright under the laws of the United States.	    */
/*  This  program is confidential, proprietary, and  a	trade secret,	    */
/*  not  to  be disclosed  without written authorization from Pocomatic     */
/*  Software, Inc.  Any use,  duplication,  or disclosure of this program   */
/*  by other than  Pocomatic Software Inc. and its assigned licensees	    */
/*  is strictly forbidden by law.					    */
/*									    */
/*  Author: Ke Jin <kejin@pocomatic.com>				    */
/*									    */
/****************************************************************************/
#ifndef _poco_dtd_resolver_h
# define _poco_dtd_resolver_h

#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/sax/InputSource.hpp>

XERCES_CPP_NAMESPACE_USE

class POCO_DtdResolver : public EntityResolver {
  public:
	InputSource* resolveEntity(
		const XMLCh *const	publicId,
		const XMLCh *const   	systemId);
};

#endif
