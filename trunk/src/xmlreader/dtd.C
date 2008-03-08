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

const char* pocomatic_get_dtd()
{
	return
	"<?xml version='1.0' encoding='UTF-8'?>\n"
	"<!ELEMENT poco-application-context ((header|bean|import|load)*)>\n"
	"<!ATTLIST poco-application-context id	CDATA	#IMPLIED>\n"
	"<!ELEMENT bean (method-arg*, on-error?, (ioc|bean)*)>\n"
	"<!ATTLIST bean id	ID	#IMPLIED>\n"
	"<!ATTLIST bean class CDATA	#REQUIRED>\n"
	"<!ATTLIST bean factory-bean	IDREF	#IMPLIED>\n"
	"<!ATTLIST bean factory-method	CDATA	#IMPLIED>\n"
	"<!ATTLIST bean factory-wrapper	CDATA	#IMPLIED>\n"
	"<!ATTLIST bean dup-method	CDATA	#IMPLIED>\n"
	"<!ATTLIST bean self-dup-method	CDATA	#IMPLIED>\n"
	"<!ATTLIST bean destroy-bean	IDREF	#IMPLIED>\n"
	"<!ATTLIST bean self-destroy-method	CDATA	#IMPLIED>\n"	
	"<!ATTLIST bean destroy-method	CDATA	#IMPLIED>\n"
	"<!ATTLIST bean singleton (true|false) 'true'>\n"
	"<!ATTLIST bean lazy-init (true|false) 'true'>\n"
	"<!ATTLIST bean abstract  (true|false) 'false'>\n"
	"<!ATTLIST bean depends-on	IDREF	#IMPLIED>\n"
	"<!ATTLIST bean key		CDATA	#IMPLIED>\n"
	"<!ATTLIST bean label		CDATA	#IMPLIED>\n"
	"<!ELEMENT method-arg	(bean|ref|this-bean|array)?>\n"
	"<!ENTITY % poco-basic-types 'byte|char|uchar|short|ushort|long|ulong|float|double|string|cstring|bean'>\n"
	"<!ATTLIST method-arg type	(%poco-basic-types;|array)	'bean'>\n"
	"<!ATTLIST method-arg class	CDATA		#IMPLIED>\n"
	"<!ATTLIST method-arg ref	IDREF		#IMPLIED>\n"
	"<!ATTLIST method-arg value	CDATA		#IMPLIED>\n"
	"<!ENTITY % syntax 'ptr|dup|deref'>\n"
	"<!ATTLIST method-arg pass	(%syntax;)	'ptr'>\n"
	"<!ELEMENT on-error (method-arg*)>\n"
	"<!ATTLIST on-error method	CDATA		#REQUIRED>\n"
	"<!ELEMENT ioc (index-arg?, method-arg*)>\n"
	"<!ELEMENT index-arg (bean|ref)?>\n"
	"<!ATTLIST index-arg type	(%poco-basic-types;)	#REQUIRED>\n"
	"<!ATTLIST index-arg class	CDATA		#IMPLIED>\n"
	"<!ATTLIST index-arg ref	IDREF		#IMPLIED>\n"
	"<!ATTLIST index-arg value	CDATA		#IMPLIED>\n"
	"<!ATTLIST index-arg pass	(%syntax;)	'ptr'>\n"
	"<!ATTLIST ioc method CDATA		#REQUIRED>\n"
	"<!ATTLIST ioc target (this-bean|none)	#IMPLIED>\n"
	"<!ELEMENT ref EMPTY>\n"
	"<!ATTLIST ref bean		IDREF		#IMPLIED>\n"
	"<!ELEMENT array (item)*>\n"
	"<!ATTLIST array type		(%poco-basic-types;|namevalue)	'bean'>\n"
	"<!ATTLIST array class		CDATA		#IMPLIED>\n"
	"<!ELEMENT item (bean|ref|this-bean)?>\n"
	"<!ATTLIST item name 		CDATA #IMPLIED>\n"
	"<!ATTLIST item ref 		IDREF #IMPLIED>\n"
	"<!ATTLIST item value		CDATA #IMPLIED>\n"
	"<!ELEMENT import EMPTY>\n"
	"<!ATTLIST import resource	CDATA #REQUIRED>\n"
	"<!ATTLIST import type		CDATA #IMPLIED>\n"
	"<!ELEMENT header (#PCDATA)>\n"
	"<!ELEMENT load	EMPTY>\n"
	"<!ATTLIST load library	CDATA	#REQUIRED>\n"
	"<!ELEMENT this-bean		EMPTY>\n";
}
