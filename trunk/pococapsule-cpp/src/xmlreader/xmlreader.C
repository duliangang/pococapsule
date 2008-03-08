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

#include <xmlreader.h>
#include "pocodom.h"
#include "table.h"
#include "util.h"
#include "pocodtdres.h"

#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>

static DefaultErrorHandler*	_error_handler = new DefaultErrorHandler;

static int retcode = pocomatic_register_desc_reader(new POCO_XML_Reader);

POCO_DOM::Element* POCO_XML_Reader::read(const char* file, POCO_AppEnv* env)
{
	return internal_read(file, -1, env);
}

POCO_DOM::Element* POCO_XML_Reader::read(const char* buf, int len, POCO_AppEnv* env)
{
	return internal_read(buf, len, env);
}

const char* POCO_XML_Reader::tid()
{
	return "xml";
}

POCO_DOM::Element* POCO_XML_Reader::internal_read(const char* src, int len, POCO_AppEnv* env)
{
	if( env == NULL ) {
		env = POCO_AppContext::getDefaultAppEnv();
	}

	 XMLPlatformUtils::Initialize();
	_error_handler = new DefaultErrorHandler;
	_error_handler->set_filename(src);

	_error_handler->resetErrors();
	POCO_DOM::Element* pocoelem = NULL;

	if( len == -1 ) {
		XMLCh* xmlstr = XMLString::transcode(src);
		LocalFileInputSource istrm(xmlstr);
		pocoelem = _read(istrm, env);
		XMLString::release(&xmlstr);
	}
	else {
		if( len == 0 ) {
			len = strlen(src);
		}

		MemBufInputSource istrm((const XMLByte*)src, len, "xml-str");
		pocoelem = _read(istrm, env);
	}

	XMLPlatformUtils::Terminate();

	if( _error_handler->getErrorCount() != 0 ) {
		delete pocoelem;
		return NULL;
	}

	return pocoelem;
}

POCO_DOM::Element* POCO_XML_Reader::_read(InputSource& istrm, POCO_AppEnv* env)
{
	XercesDOMParser* parser = new XercesDOMParser;
	parser->setValidationScheme(XercesDOMParser::Val_Auto);
	parser->setErrorHandler(_error_handler);
	POCO_DtdResolver resolver;
	parser->setEntityResolver(&resolver);

	int _init_error_count = _error_handler->getErrorCount();
	DOMNode* doc = NULL;

	try {
		parser->parse(istrm);
		doc = parser->getDocument();
	}
	catch(const SAXParseException& saxe) {
		_error_handler->fatalError(saxe);
	}
	catch(const DOMException& dome) {
		POCO_String msg 
			= POCO_XML_Reader::transcode(dome.getMessage());
		env->fatal_error(msg.in());
	}
	catch(const XMLException& xmle) {
		POCO_String msg 
			= POCO_XML_Reader::transcode(xmle.getMessage());
		env->fatal_error(msg.in());
	}

	POCO_DOM::Element* pocoelem = NULL;

	if( doc != NULL
	 && (_error_handler->getErrorCount() == _init_error_count) ) {
		DOMNodeList* list = doc->getChildNodes();
		for(unsigned i=0;list && i<list->getLength();i++) {
			DOMNode* node = list->item(i);

			if( node->getNodeType() == DOMNode::ELEMENT_NODE ) {
				pocoelem = _create_poco_element(node, NULL);
				break;
			}
		}
	}

	delete parser;

	return pocoelem;
}

POCO_DOM::Element* 
POCO_XML_Reader::_create_poco_element(
	const DOMNode* dom_node, POCO_DOM::Element* parent)
{
	POCO_DOM::Element* elem = new POCO_DOM::Element(parent);

	elem->text_content = POCO_XML_Reader::transcode(dom_node->getTextContent());
	elem->tag_name = POCO_XML_Reader::transcode(dom_node->getNodeName());

	DOMNamedNodeMap* dom_attrs = dom_node->getAttributes();

	elem->num_attrs = (dom_attrs==NULL)? 0:dom_attrs->getLength();

	if( elem->num_attrs != 0 ) {
		elem->attrs = new POCO_DOM::Attribute*[elem->num_attrs];

		for(int i=0;i<elem->num_attrs;i++) {
			DOMNode* dom_attr = dom_attrs->item(i);
			elem->attrs[i] = new POCO_DOM::Attribute;

			elem->attrs[i]->name = POCO_XML_Reader::
					transcode(dom_attr->getNodeName());
			POCO_String str = POCO_XML_Reader::
					transcode(dom_attr->getNodeValue());

			elem->attrs[i]->value 
					= POCO_Util::trim_string(str.in());
		}
	}

	DOMNodeList* list = dom_node->getChildNodes();
	int num = (list==NULL)? 0:list->getLength();

	if( num == 0 ) {
		return elem;
	}

	POCO_Table table;

	for(int i=0; i<num; i++) {
		DOMNode* dom_child = list->item(i);

		if( dom_child->getNodeType() != DOMNode::ELEMENT_NODE ) {
			continue;
		}

		POCO_DOM::Element* child 
			= POCO_XML_Reader::_create_poco_element(dom_child, elem);
		table.put(child);
	}

	elem->num_child_elems = table.count();

	if( table.count() != 0 ) {
		elem->child_elems = new POCO_DOM::Element*[table.count()];

		for(int i=0;i<table.count();i++) {
			elem->child_elems[i] = (POCO_DOM::Element*)table.get(i);
		}
	}

	return elem;
}

//
// Utility function, to translate XML string into char*.
//
char* POCO_XML_Reader::transcode(const XMLCh* xml)
{
	char* str = XMLString::transcode(xml);

	POCO_String pocostr = (const char*)str;
	XMLString::release(&str);
	return pocostr.retn();
}

DefaultErrorHandler* POCO_XML_Reader::set_parser_error_handler(DefaultErrorHandler* new_err_handler)
{
	DefaultErrorHandler* old_handler = _error_handler;
	_error_handler = new_err_handler;

	return old_handler;
}
