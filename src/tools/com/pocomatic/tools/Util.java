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

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.Attr;
import org.w3c.dom.NodeList;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Document;
import org.w3c.dom.ProcessingInstruction;
import org.w3c.dom.Comment;
import org.w3c.dom.Text;
import java.io.PrintStream;

class Util {
  static String getTextContent(Node node) {
    NodeList list = node.getChildNodes();

    String text = "";
    int count = 0;

    for(int i=0; list!=null && i<list.getLength(); i++) {
      String value = list.item(i).getNodeValue();

      if( value != null ) {
        text += value;
        count++;
      }
    }

    if( count == 0 ) {
      return null;
    }

    return text;
  }

  static Element getFirstChildOfTagName(Element elem, String name) {
    NodeList nodeList = elem.getChildNodes();

    if( nodeList == null ) {
      return null;
    }

    for(int i=0; i<nodeList.getLength(); i++) {
      Node node = nodeList.item(i);

      if( node instanceof Element ) {
        elem = (Element)node;
        if( elem.getTagName().equals(name) ) {
          return elem;
        }
      }
    }

    return null;
  }

  static int numberOfChildOfTagName(Element elem, String name) {
    int count = 0;
    NodeList nodeList = elem.getChildNodes();

    for(int i=0; i<nodeList.getLength(); i++) {
      Node node = nodeList.item(i);

      if( node instanceof Element ) {
        elem = (Element)node;
        if( elem.getTagName().equals(name) ) {
          count++;
        }
      }
    }

    return count;
  }

  static String trimLeadingWhitespace(String str) {
    if( str == null || str.length() == 0 ) {
      return str;
    }

    StringBuffer buf = new StringBuffer(str);
    for(;;) {
      if( buf.length() == 0 
       || Character.isWhitespace(buf.charAt(0)) == false ) {
        break;
      }
      buf.deleteCharAt(0);
    }

    return buf.toString();
  } 

  static String trimTrailingWhitespace(String str) {
    if( str == null || str.length() == 0 ) {
      return str;
    }

    StringBuffer buf = new StringBuffer(str);
    for(;;) {
      int len = buf.length();
      if( len == 0
       || Character.isWhitespace(buf.charAt(len - 1)) == false ) {
        break;
      }
      buf.deleteCharAt(len-1);
    }

    return buf.toString();
  }

  static String trimTrailingChar(String str) {
    if( str == null || str.length() == 0 ) {
      return str;
    }

    StringBuffer buf = new StringBuffer(str);
    int len = buf.length();
    buf.deleteCharAt(len-1);

    for(;;) {
      len = buf.length();
      if( len == 0
       || Character.isWhitespace(buf.charAt(len - 1)) == false ) {
        break;
      }
      buf.deleteCharAt(len-1);
    }

    return buf.toString();
  }

  static String trimLeadingChar(String str) {
    if( str == null || str.length() == 0 ) {
      return str;
    }

    StringBuffer buf = new StringBuffer(str);
    buf.deleteCharAt(0);

    return trimLeadingWhitespace(buf.toString());
  }

  static String trimOuterWhitespace(String str) {
    if( str == null || str.length() == 0 ) {
      return str;
    }

    String ret = trimLeadingWhitespace(str);
    ret = trimTrailingWhitespace(ret);

    return ret;
  }

  static boolean compare(String a, String b) {
    if( a == null ) {
      return (b == null);
    }

    if( b == null ) {
      return (a == null);
    }

    return a.equals(b);
  }

  static boolean compare(ArgumentDesc[] a, ArgumentDesc[] b) {
    if( a == null ) {
      return (b == null);
    }

    if( b == null ) {
      return (a == null);
    }

    if( a.length != b.length ) {
      return false;
    }

    for(int i=0; i<a.length; i++) {
      if( a[i] == null && b[i] != null ) {
        return false;
      }
      
      if( b[i] == null && a[i] != null ) {
        return false;
      }

      if( a[i] == null && b[i] == null ) {
        continue;
      }

      if( a[i].equals(b[i]) == false ) {
        return false;
      }
    }

    return true; 
  }

  static boolean is_empty(String str) {
    return ( str == null || str.length() == 0 );
  }

  static String getTrimedAttribute(Element elem, String attr_name) {
    return trimOuterWhitespace(elem.getAttribute(attr_name));
  }


  static String class_to_ptr(String clazz) {
    if( clazz == null || clazz.length() == 0 || clazz.equals("void") ) {
      return null;
    }

    if( clazz.charAt(0) == '*' ) {
      clazz = trimLeadingChar(clazz);

      if( clazz == null || clazz.length() == 0 || clazz.equals("void") ) {
        return null;
      }
 
      return clazz;
    }

    return clazz + "*";
  }

  static boolean is_xml_file(String file) {
    try {
      java.io.InputStream istrm = new java.io.FileInputStream(file);
  
      byte[] buf = new byte[8];
      istrm.read(buf, 0, 8);  
      istrm.close();
      String magic = new String(buf, 0, 8);
 
      if( magic.startsWith("<?xml") ) {
        return true;
      }
    }
    catch(Exception e) {
      e.printStackTrace();
    }

    return false;
  }

  static private final String ELEM_ENDIENT = "\t";

  static void printDoc(Document doc, PrintStream pstrm) {
     String version = "1.0"; // doc.getXmlVersion();
     String encoding = null; // doc.getXmlEncoding();

     pstrm.print(
       "<?xml version=\"" + version + "\"");

     if(encoding==null) {
       pstrm.println("?>");
     }
     else {
       pstrm.println(" encoding=\"" + encoding + "\"?>");
     }

     printPI(doc, pstrm);
     pstrm.println("");
     printDoctype(doc, pstrm);
     pstrm.println("\n");
     printElem(doc.getDocumentElement(), "", true, pstrm);
  }

  static private void printPI(Document doc, PrintStream pstrm) {
    NodeList nodes = doc.getChildNodes();

    for(int i=0;i<nodes.getLength();i++) {
      Node node = nodes.item(i);

      if( node instanceof ProcessingInstruction ) {
        ProcessingInstruction pi = (ProcessingInstruction)node;
        pstrm.println("<?" + pi.getTarget() + " " +
                           pi.getData() + "?>");
      }
    }
  }

  static private void printDoctype(Document doc, PrintStream pstrm) {
    if( doc.getDoctype() != null ) {
      pstrm.println(
      "<!DOCTYPE " + doc.getDoctype().getName());

      String pubid = doc.getDoctype().getPublicId();

      if( pubid != null ) {
        pstrm.print("\tPUBLIC \"" + pubid + "\"");
      }

      String sysid = doc.getDoctype().getSystemId();

      if( sysid != null ) {
        if( pubid != null ) {
          pstrm.println("");
        }
        pstrm.print("\tSYSTEM \"" + sysid + "\"");
      }

      pstrm.print(">");
    }
  }

  static private void printElem(
             Element elem, String endient, boolean toplevel, PrintStream pstrm ) {
    if( elem == null ) {
      return;
    }

    pstrm.print(endient + "<" + elem.getTagName());
    printAttrs(elem, endient, pstrm);
    int count = printChildElems(elem, endient, toplevel, pstrm);
    if( count == 0 ) {
      String text = elem.getNodeValue(); 

      if(is_empty(text)==false) {
        pstrm.print(text);
      }

      pstrm.println("</" + elem.getTagName() + ">");
    }
    else {
      pstrm.println(endient + "</" + elem.getTagName() + ">");
    }
  }

  static private void printAttrs(Element elem, String endient, PrintStream pstrm) {
    NamedNodeMap attrs = elem.getAttributes();

    if( attrs.getLength() == 0 ) {
      pstrm.print(">");
      return;
    }

    Attr attr = (Attr)attrs.item(0);

    if( attrs.getLength() == 1 ) {
      pstrm.print(" " + attr.getName() + "=\"" 
                             + attr.getValue() + "\">");
      return;
    }

    endient += ELEM_ENDIENT;

    for(int i=0; i<attrs.getLength(); i++) {
      attr = (Attr)attrs.item(i);
      pstrm.print("\n" + endient 
                            + attr.getName() + "=\""
                            + attr.getValue() + "\"");
    }

    pstrm.print(">");
  }

  static private int printChildElems(
           Element elem, String endient, boolean toplevel, PrintStream pstrm) {
    endient += ELEM_ENDIENT;

    int count = 0;
    boolean after_comment = false;

    NodeList nodes = elem.getChildNodes();
    for(int i=0; i<nodes.getLength(); i++) {
      Node node = nodes.item(i);

      if( node instanceof Element ) {
        if( after_comment == false && (toplevel || count == 0) ) {
          pstrm.println("");
        }
        after_comment = false;
        printElem((Element)node, endient, false, pstrm);
        count++;
      }
      else
      if( node instanceof Comment ) {
        if( toplevel && count != 0 ) {
          after_comment = true;
          pstrm.println("");
        }
        Comment comm = (Comment)node;

        String data = comm.getData();
        String commEnd;
 
        if( data.indexOf('\n') == -1
         && data.indexOf('\r') == -1 ) {
          commEnd = "-->";
        }
        else {
          commEnd = "\n" + endient + "-->";
        }

        String head = "\n";
        if( toplevel ) {
          head = "";
        }
        
        pstrm.println(head + endient + "<!--" + data + commEnd);
      }
      else
      if( node instanceof Text ) { // && elem.getTagName().equals("header") ) {
        pstrm.print(node.getNodeValue());
      }
    }

    return count;
  }

  static void printElemShallow(
             Element elem, String endient, PrintStream pstrm ) {
    if( elem == null ) {
      return;
    }

    String tag = elem.getTagName();
    if( tag.equals("bean") == false && tag.equals("ioc") == false ) {
      return;
    }

    pstrm.print(endient + "<" + elem.getTagName());
    printAttrs(elem, endient, pstrm);
    int count = printMethodArgs(elem, endient, pstrm);
    if( count == 0 ) {
      pstrm.println();
    }
    pstrm.println( endient + "</" + tag + ">");
  }

  static private int printMethodArgs(
           Element elem, String endient, PrintStream pstrm) {
    endient += ELEM_ENDIENT;

    int count = 0;

    NodeList nodes = elem.getChildNodes();
    for(int i=0; i<nodes.getLength(); i++) {
      Node node = nodes.item(i);

      if( node instanceof Element ) {
        Element child = (Element)node;
        String tag = child.getTagName();
        if( tag.equals("method-arg") ) {
          if( count == 0 ) {
            pstrm.println();
          }
          printElem((Element)node, endient, true, pstrm);
          count++;
        }
      }
    }

    return count;
  }
}
