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
import org.w3c.dom.Element;
import org.w3c.dom.Attr;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.NamedNodeMap;
import java.io.OutputStream;

public class DescEncode {
  private static final char DELIM = '\0';

  public static void encodeElement(Element elem, java.io.PrintStream ostrm) {
    encodeElement(elem, ostrm, null);
  }

  public static void encodeElement(Element elem, java.io.PrintStream ostrm, String suffix) {
    ostrm.print(elem.getTagName()); ostrm.print(DELIM);
    int count;

    NodeList nodeList = elem.getChildNodes();

    int num_child_elems = 0;
    for(int i=0; i<nodeList.getLength(); i++) {
      Node node = nodeList.item(i);

      if( node instanceof Element ) {
        num_child_elems++;
      }
    }

    //
    // text content
    //
    count = 0;
    if( num_child_elems == 0 ) {
      String text = elem.getNodeValue();
      if( Util.is_empty(text) == false ) {
        count = 1;
        for(int i=0;i<text.length();i++) {
          if( text.charAt(i) == '\n' ) {
            count++;
          }
        }
        ostrm.print(count); ostrm.print(DELIM);
        ostrm.print(text); ostrm.print(DELIM);
      }
    }

    if( count == 0 ) {
      // no content
      ostrm.print("0"); ostrm.print(DELIM);
    }

    //
    // all attributes
    //
    NamedNodeMap attrMap = elem.getAttributes();

    ostrm.print(attrMap.getLength()); ostrm.print(DELIM);

    boolean flag = false;

    if( suffix != null && elem.getTagName().equals("import") ) {
      flag = true;
    }

    for(int i=0; i<attrMap.getLength(); i++) {
      Attr attr = (Attr)attrMap.item(i);
      ostrm.print(attr.getName()); ostrm.print(DELIM);
      
      String val = attr.getValue();

      if( flag && attr.getName().equals("resource") ) {
        int idx = val.lastIndexOf('.');
        if( idx != -1 ) {
          val = val.substring(0, idx);
        }
        val += suffix;
      }

      ostrm.print(val); ostrm.print(DELIM);
    }

    //
    // all child elements
    //
    ostrm.print(num_child_elems); ostrm.print(DELIM);

    for(int i=0;i<nodeList.getLength(); i++) {
      Node node = nodeList.item(i);

      if( node instanceof Element ) {
        encodeElement((Element)node, ostrm, suffix);
      }
    }

    ostrm.flush();
  }
};
