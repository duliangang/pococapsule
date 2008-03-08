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
import org.w3c.dom.Text;

import java.io.FileOutputStream;
import java.io.PrintStream;
import java.io.ByteArrayOutputStream;

public class Encode {
  public static void encode(String[] args) {
      System.out.println(
         "----------------------------------------------------------------\n"
       + "  Pocomatic Software, Applicaiton Context Encoder, version 1.0  \n"
       + "----------------------------------------------------------------\n");

      if( args.length == 1 ) {
        printEncodeUsage(args[0]);
        return;
      }

      boolean rec = false;
      String ext = "_poco.ctx";

      for(int i=0; i<args.length; i++) {
        if( args[i].startsWith("-I=") ) {
          System.setProperty("user.dir", args[i].substring(3));
        }
        else
        if( args[i].equals("-rec") || args[i].equals("-r") ) {
          rec = true;
        }
        else
        if( args[i].startsWith("-suffext=") ) {
          ext = args[i].substring(9);
        }
        else
        if( args[i].startsWith("-s=") ) {
          ext = args[i].substring(3);
        }
	else
        if( args[i].equals("-help") ) {
          printEncodeUsage(args[0]);
        }
        else
        if( args[i].startsWith("-") ) {
          System.out.println("invalid option: " + args[i]);
          printEncodeUsage(args[0]);
          return;
        }
      }

      for(int i=1; i<args.length; i++) {
        if( args[i].startsWith("-") ) {
          continue;
        }

        if( Util.is_xml_file(args[i]) == false ) {
          System.out.println("target " + args[i] + " is not a valid resource file, skipped");
          continue;
        }

        do_encode(args[i], ext, rec, false, false, null, true);
      }
  }

  public static void transform(String[] args) {
      System.out.println(
         "-----------------------------------------------------------------------\n"
       + "  Pocomatic Software, Application Description Transformer, version 1.0  \n"
       + "-----------------------------------------------------------------------\n");

      if( args.length == 1 ) {
        printTransUsage(args[0]);
        return;
      }

      boolean rec = false;
      boolean expendOnly = false;
      String ext = "_poco.xml";
      java.util.ArrayList headers = new java.util.ArrayList();
      boolean validation = true;

      for(int i=0; i<args.length; i++) {
        if( args[i].equals("-E") ) {
          expendOnly = true;
        }
        else
        if( args[i].startsWith("-I=") ) {
          System.setProperty("user.dir", args[i].substring(3));
        }
        else
        if( args[i].startsWith("-suffext=") ) {
          ext = args[i].substring(9);
        }
        else
        if( args[i].startsWith("-s=") ) {
          ext = args[i].substring(3);
        }
	else
	if( args[i].equals("-c") ) {
	  validation = false;
	}
        else
        if( args[i].startsWith("-h=") ) {
          String hdr = args[i].substring(3);
          if( Util.is_empty(hdr) == false ) {
            headers.add(hdr);
          }
        }
        else
        if( args[i].equals("-help") ) {
          printTransUsage(args[0]);
        }
        else
        if( args[i].startsWith("-") ) {
          System.out.println("invalid option: " + args[i]);
          printTransUsage(args[0]);
          return;
        }
      }

      for(int i=1; i<args.length; i++) {
        if( args[i].startsWith("-") ) {
          continue;
        }

        if( Util.is_xml_file(args[i]) == false ) {
          System.out.println("target " + args[i] + " is not a valid resource file, skipped");
          continue;
        }

        do_encode(args[i], ext, rec, expendOnly, true, headers, validation);
      }
  }

  static void printEncodeUsage(String cmd) {
    System.out.println(
                 "Usage:\n\n"
               + "	" + cmd + " (xml-file|option)*\n\n"
               + "options:\n"
               + "	-help                   : print this page\n"
               + "	-s=[name]               : suffix and extend name of encode file,\n" 
               + "	                          default value is '_poco.ctx'\n");
  }

  static void printTransUsage(String cmd) {
    System.out.println(
                 "Usage:\n\n"
               + "	" + cmd + " (xml-file|option)*\n\n"
               + "options:\n"
               + "	-help                   : print this page\n"
               + "	-s=[name]               : suffix and extend name of encode file,\n" 
               + "	                          default value is '_poco.xml' for transformation,\n"
           //    + "	-h=[text]               : add the text as a <header>[text]</header> node\n"
               + "	-E                      : print the transformed result only\n"
               + "	-c                      : disable schema validating\n");
  }

  static private java.util.Set ctxtIds;

  static {
    ctxtIds = new java.util.HashSet();
  }

  //
  // recursively append all imported resources
  //
  static private void deepExpendDoc(Document doc, boolean validation) throws Exception {
    Element root = doc.getDocumentElement();

    if( root == null ) {
      return;
    }

    String attr = root.getAttribute("id");
    if( Util.is_empty(attr) == false ) {
      ctxtIds.add(attr);
    }

    NodeList nodeList = root.getChildNodes();

    for(int i=0; i<nodeList.getLength(); i++) {
      Node node = nodeList.item(i);

      if( node instanceof Element ) {
        Element elem = (Element)node;

        if( elem.getTagName().equals("import") ) {
          attr = elem.getAttribute("resource");

          if( Util.is_xml_file(attr) == false ) {
            System.out.println("import resource " + attr + " is not a xml file, skipped");
            continue;
          }

          Document imported_doc = DocBuilder.xml_to_dom(attr, validation);

          if( imported_doc == null ) {
            continue;
          }

          Element imported_root = imported_doc.getDocumentElement();
          String ctxtId = imported_root.getAttribute("id");
          if( Util.is_empty(ctxtId) == false && ctxtIds.contains(ctxtId) ) {
            System.out.println("imported resource " + attr + 
                               " contains a duplicated context (id=" + 
                               ctxtId + "), skipped");
            continue;
          }

          System.out.println("imported root is " + imported_root);

          node.appendChild(imported_root);
          System.out.println("Appended imported resource " + attr + " into the context");
          deepExpendDoc(imported_doc, validation);
        }
      }
    }
  }

  public static String parse_xml_file(String xmlfile) {
    String s = "Error:";

    try {
      Document doc = DocBuilder.xml_to_dom(xmlfile, true);
      ByteArrayOutputStream ostrm = new ByteArrayOutputStream();
      HexEncodeStream hstrm = new HexEncodeStream(ostrm);
      DescEncode.encodeElement(doc.getDocumentElement(), new PrintStream(hstrm));

      // Util.printDoc(doc, new PrintStream(hstrm));
      return ostrm.toString();
    }
    catch(Exception e) {
      s += e.getMessage();
    }

    return s;
  }

  public static String parse_xml_string(String xml) {
    String s = "Error:";

    try {
      Document doc = DocBuilder.xml_string_to_dom(xml);
      ByteArrayOutputStream ostrm = new ByteArrayOutputStream();
      HexEncodeStream hstrm = new HexEncodeStream(ostrm);
      DescEncode.encodeElement(doc.getDocumentElement(), new PrintStream(hstrm));

      // Util.printDoc(doc, new PrintStream(hstrm));
      return ostrm.toString();
    }
    catch(Exception e) {
      s += e.getMessage();
    }

    return s;
  }

  private static void do_encode(String inputxml, String extname, 
                                boolean rec, boolean expendOnly, boolean transOnly,
				java.util.ArrayList headers,
				boolean validation) {
    try {
      int idx = inputxml.lastIndexOf('.');
      String outputfile = inputxml;
      if( idx != -1 ) {
        outputfile = outputfile.substring(0, idx);
      }
      outputfile += extname;

      if( expendOnly == false ) {
        if( transOnly ) {
          System.out.println("writing the transformed xml to " + outputfile + " ...");
        }
        else {
          System.out.println("generating encoded context description: " 
                         + outputfile + " ....");
        }
      }

      Document doc = DocBuilder.xml_to_dom(inputxml, validation);

      if( rec == true ) {
        deepExpendDoc(doc, validation);
      }

      Element root = doc.getDocumentElement();

      if( headers != null && headers.size() != 0 ) {
        NodeList nodes = doc.getElementsByTagName("poco-application-context");

        if( nodes != null && nodes.getLength() != 0 ) {
          Node node = nodes.item(0);
            
          for(int i=0; i<headers.size(); i++) {
            String header = (java.lang.String)headers.get(i);
            if( header == null ) continue;

            Element e = doc.createElement("header");
            Text t = doc.createTextNode(header);
            e.appendChild(t);
            node.appendChild(e);
          }
	}
      }

      if( expendOnly ) {
        Util.printDoc(doc, System.out);
        return;
      }
      else 
      if( transOnly ) {
        Util.printDoc(doc, new PrintStream(new FileOutputStream(outputfile)));
        return;
      }

      FileOutputStream fout = new FileOutputStream(outputfile);
      HexEncodeStream hstrm = new HexEncodeStream(fout);
      PrintStream pstrm = new PrintStream(hstrm);

      DescEncode.encodeElement(root, pstrm, extname);
      pstrm.flush();
      fout.close();
    } 
    catch(Exception e) {
      e.printStackTrace();
    }
  }
}

