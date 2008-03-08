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
#ifndef _poco_xml_reader_h
# define _poco_xml_reader_h

#if defined(WIN32)
#  if defined(BUILD_POCOXMLREADER_DLL)
#    define _POCO_XML_EXPORT __declspec(dllexport)
#  else
#    define _POCO_XML_EXPORT __declspec(dllimport)
#  endif
#else
#  define _POCO_XML_EXPORT
#endif

# include "pocodom.h"
# include "pocostr.h"
# include <stdio.h>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/deprecated/DOMString.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>

XERCES_CPP_NAMESPACE_USE

class DefaultErrorHandler;

extern const char* pocomatic_get_dtd();

class _POCO_XML_EXPORT POCO_XML_Reader : public POCO_AppDescReader {
	friend class POCO_DtdResolver;

	static DefaultErrorHandler* set_parser_error_handler(DefaultErrorHandler* new_handler);
	static POCO_DOM::Element* internal_read(const char* src, int len, POCO_AppEnv*);


	friend class DefaultErrorHandler;

  private:
	static POCO_DOM::Element* _create_poco_element(
			const DOMNode* node, POCO_DOM::Element* parent = NULL);
	static char* transcode(const XMLCh* xml);
	static POCO_DOM::Element* _read(InputSource&, POCO_AppEnv*);
  public:
	const char*	   tid();
	POCO_DOM::Element* read(const char* file, POCO_AppEnv*);
	POCO_DOM::Element* read(const char* buf, int len, POCO_AppEnv*);
};

class DefaultErrorHandler : public ErrorHandler {
	int	_count;
	const char* _file_name;
  public:
	DefaultErrorHandler() : _count(0), _file_name("unknown") {}
	void warning(const SAXParseException& e) {
		POCO_String msg = POCO_XML_Reader::transcode(e.getMessage());

		// Xerces-C issues warning on referring undeclared dtd element.
		/*
		fprintf(stderr,
			"Warning (file %s (or its dtd file), "
			"line %d, column %d): %s\n",
			_file_name,
			(int)e.getLineNumber(),
			(int)e.getColumnNumber(),
			msg.in());
		*/
	}

	void error(const SAXParseException& e) {
		POCO_String msg = POCO_XML_Reader::transcode(e.getMessage());

		fprintf(stderr,
			"Error (file %s (or its dtd file), "
			"line %d, column %d): %s\n",
			_file_name,
			(int)e.getLineNumber(),
			(int)e.getColumnNumber(),
			msg.in());
		_count++;
	}

	void fatalError(const SAXParseException& e) {
		POCO_String msg = POCO_XML_Reader::transcode(e.getMessage());

		fprintf(stderr,
			"Fatal error (file %s (or its dtd file), "
			"line %d, column %d): %s\n",
			_file_name,
			(int)e.getLineNumber(),
			(int)e.getColumnNumber(),
			msg.in());
		_count++;
	}

	void resetErrors() { _count = 0; }
	int getErrorCount() const { return _count; }
	const char* set_filename(const char* file_name) {
			const char* ret = _file_name;
			_file_name = file_name;
			return ret; }
};

#endif
