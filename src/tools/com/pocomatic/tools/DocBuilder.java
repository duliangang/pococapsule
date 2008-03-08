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

package com.pocomatic.tools;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import org.w3c.dom.ProcessingInstruction;
import org.w3c.dom.DocumentType;

import java.io.File;
import java.io.FileInputStream;
import javax.xml.transform.stream.StreamSource;

import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.dom.DOMSource;

class DocBuilder {
  static Document parse(String xmlfile) throws Exception {
    return parse(xmlfile, true);
  }

  static Document parse(String xmlfile, boolean validation) throws Exception {
    if( validation == false ) {
      _factory.setValidating(false);
    }
    DocumentBuilder builder = _factory.newDocumentBuilder();
    _factory.setValidating(true);
    builder.setErrorHandler(new ErrorHandlerImpl(xmlfile));
    builder.setEntityResolver(new DtdResolver());

    java.io.FileInputStream istrm = new java.io.FileInputStream(xmlfile);

    return builder.parse(istrm);
  }

  static  Document parse_string(String xml) throws Exception {
    DocumentBuilder builder = _factory.newDocumentBuilder();
    builder.setErrorHandler(new ErrorHandlerImpl(xml));
    builder.setEntityResolver(new DtdResolver());

    java.io.ByteArrayInputStream istrm = new java.io.ByteArrayInputStream(xml.getBytes());

    return builder.parse(istrm);
  }

  static Document xml_to_dom(String xmlfile, boolean validation) throws Exception {
    Document doc = parse(xmlfile, validation);
    return transform(doc);
  }

  static Document xml_string_to_dom(String xml) throws Exception {
    Document doc = parse_string(xml);
    return transform(doc);
  }

  static Document transform(Document doc) throws Exception { 
    if( doc == null ) {
      return null;
    }

    NodeList nodes = doc.getChildNodes();

    String href = null;

    for(int i=0;nodes!=null && i<nodes.getLength();i++) {
      Node node = nodes.item(i);

      if( node instanceof ProcessingInstruction ) {
        ProcessingInstruction pi = (ProcessingInstruction)node;
        String target = pi.getTarget();
        String data = pi.getData();

        if( "xml-transform".equals(target) 
         || "xml-stylesheet".equals(target) ) {
          href = get_href(pi.getData());
          break;
        }
      }
    }

    if( href == null ) {
      return doc;
    }

    doc = do_transform(doc, href);

    nodes = doc.getElementsByTagName("transform-error");
    if( nodes != null && nodes.getLength() != 0 ) {
      throw new Exception(((Element)nodes.item(0)).getAttribute("message"));
    }

    // recursive
    return transform(doc);
  }

  private static String get_href(String data) {
    if(data == null) {
      return null;
    }
    int idx = 0;
    idx = data.indexOf("href=\"");
    if( idx == -1 ) {
      return null;
    }

    if( idx != 0 
     && Character.isWhitespace(data.charAt(idx-1)) == false ) {
      return null;
    }

    idx += "href=\"".length();
    int endIdx = data.indexOf('"', idx);

    if( endIdx == -1 ) {
      return null;
    }

    return data.substring(idx, endIdx);
  }

  static Document do_transform(Document doc, String xsl) throws Exception {
    String sid = null;
    String pid = null;

    if( doc.getDoctype() != null ) {
      sid = doc.getDoctype().getSystemId();
    }

    Transformer tran = get_transformer(null, xsl);

    if( tran == null ) {
      System.out.println("fail to create transformer from " + xsl);
      return doc;
    }

    DOMSource src = new DOMSource(doc);
    DOMResult res = new DOMResult();

    java.util.Properties props = tran.getOutputProperties();
    if( props != null ) {
      sid = props.getProperty("doctype-system");
      if( sid != null ) {
        res.setSystemId(sid);
      }

      pid = props.getProperty("doctype-public");
    }

    // System.out.println("Transforming according to " + xsl);
    tran.transform(src, res);

    doc = (org.w3c.dom.Document)res.getNode(); 

    if( doc.getDoctype() == null ) {
      Element elem = doc.getDocumentElement();
      if( elem != null && (sid != null || pid != null) ) {
      	DocumentType doctype 
          = doc.getImplementation().createDocumentType(
                                   elem.getTagName(), pid, sid);
        doc.appendChild(doctype);
      }
    }

    return doc;
  }

  static Transformer get_transformer(String sid, String xsl) throws Exception {
    org.xml.sax.InputSource strm = DtdResolver.resolve_resource(null, xsl);
    if( strm == null ) {
      strm = new org.xml.sax.InputSource(new FileInputStream(xsl));

      if( strm == null ) {
        throw new Exception("[P020] fail to open transform template file " + xsl);
      }
    }

    DocumentBuilder builder;

    builder = _factory2.newDocumentBuilder();
    builder.setErrorHandler(new ErrorHandlerImpl(xsl));

    org.w3c.dom.Document xslt = builder.parse(strm);
    xslt = transform(xslt);
    DOMSource src = new DOMSource(xslt);
    // src.setSystemId(sid);

    return _tfactory.newTransformer(src);
  }

  static private DocumentBuilderFactory _factory;
  static private DocumentBuilderFactory _factory2;
  static private TransformerFactory     _tfactory;
  static private  java.util.Hashtable _transformers;

  static {
    _factory = DocumentBuilderFactory.newInstance();
    _factory.setValidating(true);
    _factory.setNamespaceAware(false);
    _factory.setExpandEntityReferences(true);
    _factory2 = DocumentBuilderFactory.newInstance();
    _factory2.setNamespaceAware(true);
    _tfactory = TransformerFactory.newInstance();
   // _tfactory.setURIResolver(new URIResolverImpl());
    _transformers = new java.util.Hashtable();
  }
}
