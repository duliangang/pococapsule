/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC. All Rights Reserved.	    */
/*									    */
/*  This code is the property of Pocomatic Software LLC., and is	    */
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
#ifndef _poco_xsl_reader_h
# define _poco_xsl_reader_h

# include "domreader.h"

#if defined(WIN32)
#  if defined(BUILD_POCOXSLREADER_DLL)
#    define _POCO_XSL_EXPORT __declspec(dllexport)
#  else
#    define _POCO_XSL_EXPORT __declspec(dllimport)
#  endif
#else
#  define _POCO_XSL_EXPORT
#endif
class _POCO_XSL_EXPORT POCO_XSL_Reader : public POCO_DOM_Reader {
  public:
	const char*	   tid();
	POCO_DOM::Element* read(const char* file, POCO_AppEnv*);
	POCO_DOM::Element* read(const char* buf, int len, POCO_AppEnv*);
};

#endif
