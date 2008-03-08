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

public class Project {
  public static void project(String[] args) {
      System.out.println(
                   "------------------------------------------------------------\n"
                 + "  Pocomatic Software, Dynamic Proxy Generator, version 1.0\n"
                 + "------------------------------------------------------------\n");

      if( args.length == 1 ) {
        printUsage(args[0]);
        return;
      }

      boolean note = false;
      int flag = IGNORE_IMPORT;
      String ext = "_reflx.cc";

      java.util.ArrayList headers = new java.util.ArrayList();

      for(int i=1; i<args.length; i++) {
        if( args[i].startsWith("-r=") ) {
          String s = args[i].substring(3);

          if( s.equals("gather") ) {
            flag = GATHER_IMPORT;
          }
          else 
          if( s.equals("scatte") ) {
            flag = SCATTE_IMPORT;
          }
          else {
            System.out.println("Invalide option: " + args[i]);
            return;
          }
        }
        else
        if( args[i].startsWith("-I=") ) {
          System.setProperty("user.dir", args[i].substring(3));
        }
        else
        if( args[i].startsWith("-h=") ) {
          String fileName = args[i].substring(3);
          if( Util.is_empty(fileName) == false ) {
            String hdr = "#include \"" + fileName + "\""; 
            headers.add(hdr);
          }
        }
        else
        if( args[i].startsWith("-H=") ) {
          String fileName = args[i].substring(3);
          if( Util.is_empty(fileName) == false ) {
            String hdr = "#include <" + fileName + ">";
            headers.add(hdr);
          }
        }
	else
        if( args[i].startsWith("-x=") ) {
          String text = args[i].substring(3);
          if( Util.is_empty(text) == false ) {
            headers.add(text);
          }
        }
        else
        if( args[i].startsWith("-suffext=") ) {
          ext = args[i].substring(9);

          if( Util.is_empty(ext) ) {
            ext = "_reflx.cc";
          }
        }
        else
        if( args[i].startsWith("-s=") ) {
          ext = args[i].substring(3);

          if( Util.is_empty(ext) ) {
            ext = "_reflx.cc";
          }
        }
        else
        if( args[i].equals("-help") ) {
          printUsage(args[0]);
        }
        else
        if( args[i].equals("-note") ) {
          note = true;
        }
        else
        if( args[i].startsWith("-") ) {
          System.out.println("invalid option: " + args[i]);
          return;
        } 
      }

      if( flag == IGNORE_IMPORT ) {
        System.out.println("Imported xml resources, if any, are ignored. To recursively generate  \n"
                         + "on imported resources, please use the -r=[scatte|gather] option\n");
      }

      for(int i=1; i<args.length; i++) { 
        if( args[i].startsWith("-") ) {
          continue; 
        }

        if( Util.is_xml_file(args[i]) == false ) {
          System.out.println("target " + args[i] + " is not a xml file, skipped");
          continue;
        }

        project(args[i], headers, ext, flag, note);
     }
  }

  static void printUsage(String cmd) {
    System.out.println(
                 "Usage:\n\n"
               + "	" + cmd + " (xml-file|option)*\n\n"
               + "options:\n"
               + "	-help			: print this page\n"
               + "	-s=[name]		: suffix and extend name of proxy file,\n"
               + "	                          default value is '_reflx.cc'\n"
               + "	-H=[std-header-file]	: use the specified standard header file\n"
               + "	-h=[usr-header-file]	: use the specified user defined header file\n"
               + "	-r=[gather|scatte]	: recursively step into imported resources and gather or scatte\n"
               + "	                  	  proxy generation result into one or multiple individual proxy file(s) \n");
  }

  private final static int IGNORE_IMPORT = 0;
  private final static int SCATTE_IMPORT = 1;
  private final static int GATHER_IMPORT = 2;

  //
  // recursively pass all imported resources
  //
  static private void deepParseDoc(Document doc, Parser parser) throws Exception {
    parser.parseDoc(doc);

    Element root = doc.getDocumentElement();
    NodeList nodeList = root.getChildNodes();

    for(int i=0; i<nodeList.getLength(); i++) {
      Node node = nodeList.item(i);

      if( node instanceof Element ) {
        Element elem = (Element)node;

        if( elem.getTagName().equals("import") ) {
          String attr = elem.getAttribute("resource");

          if( Util.is_xml_file(attr) == false ) {
            System.out.println("import resource " + attr + " is not a xml file, skipped");
            continue;
          }

          doc = DocBuilder.xml_to_dom(attr, true);

          if( doc == null ) {
            continue;
          }

          root = doc.getDocumentElement();
          String ctxtId = root.getAttribute("id");
          if( Util.is_empty(ctxtId) == false && parser.contains_context(ctxtId) ) {
            System.out.println("imported resource " + attr + " contains a duplicated context (id=" + ctxtId + "), skipped");
            continue;
          }

          System.out.println("Gathering resource " + attr + " into the context");
          deepParseDoc(doc, parser);
        }
      }
    }
  }

  private static void project(String inputxml, java.util.ArrayList headers, String extname, int flag, boolean note) {
    try {
      String outputfile = inputxml;
      int idx = outputfile.lastIndexOf('.');
      if( idx != -1 ) {
        outputfile = outputfile.substring(0, idx);
      }
      outputfile += extname;
      System.out.println("generating dynamic proxy code: " + outputfile + " ....");

      java.io.OutputStream ostrm = new java.io.FileOutputStream(outputfile);
      java.io.PrintStream pstrm = new java.io.PrintStream(ostrm);

      Document doc = DocBuilder.xml_to_dom(inputxml, true);

      if( headers != null && headers.size() != 0 ) {
        NodeList nodes = doc.getElementsByTagName("poco-application-context");

        if( nodes.getLength() != 0 ) {
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

      Parser parser = new Parser();

      if( flag == GATHER_IMPORT ) {
        deepParseDoc(doc, parser);
      }
      else {
        parser.parseDoc(doc);
      }

      java.util.Date date = new java.util.Date();

      String imported_files = "";

      if( flag == GATHER_IMPORT ) {
        imported_files = " and imported files";
      }

      pstrm.println(
          "//\n"
        + "// " + outputfile + "\n"
        + "// Generated by the Pocomatic POCO DPP (Dynamic Proxy Generator), version 1.0\n"
        + "// from " + inputxml + imported_files + "\n" 
        + "// " + date.toString() + "\n"
        + "//\n");

      pstrm.println("#include <poco.h>");
      long uid = date.getTime();
      pstrm.println("POCO_ENTRY(" + uid + ")  // preserved\n");

      pstrm.println("// ---------------------------------------\n"
                  + "//  headers from -h/H options or " + inputxml + imported_files + "\n"
                  + "// ---------------------------------------");

      for(int i=0;;i++) {
        String text = parser.get_header_text(i);
        if( text == null ) {
          break;
        }
        pstrm.println(text);
      }

      pstrm.println("");

      ProxyGen gen = new ProxyGen(note);
      gen.generateProxies(pstrm, parser.allMethods());
      pstrm.close();

      if( flag != SCATTE_IMPORT ) {
        return;
      }

      Element root = doc.getDocumentElement();
      // gen proxies for imported resource
      NodeList nodeList = root.getElementsByTagName("import");

      for(int i=0;i<nodeList.getLength();i++) {
        if( nodeList.item(i) instanceof Element ) {
          Element elem = (Element)nodeList.item(i);

          String attr = elem.getAttribute("resource");
          if( Util.is_xml_file(attr) == false ) {
            System.out.println("imported resource " + attr + " is not a xml file, skipped over");
            continue;
          }

          project(attr, null, extname, SCATTE_IMPORT, note);
        }
      }
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }
};
