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

import javax.xml.transform.TransformerException;
import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import java.io.File;
import  javax.xml.transform.URIResolver;
import org.xml.sax.InputSource;

public class URIResolverImpl implements URIResolver {
  
  private static java.util.Hashtable _uris;

  private static final String pocoLoc = "http://www.pocomatic.com/";

  static {
    _uris = new java.util.Hashtable();
    /*_uris.put(
		"http://www.pocomatic.com/corba2poco.xsl", 
		"com/pocomatic/resources/corba2poco.xsl");
    */
  }

  public Source resolve(String href, String base) throws TransformerException {
    return resolve_href(href, base);
  }

  public static Source resolve_href(String href, String base) throws TransformerException {
    if( href == null ) {
      return null;
    }

    System.out.println("href: " + href);
    System.out.println("base: " + base);

      String sysid = "/home/kejin/workspace/pococapsule-cpp/src/tools/com/pocomatic/resources/utils.xsl";

//"jar:file:///home/kejin/workspace/pococapsule-cpp/lib/pocotools.jar!/com/pocomatic/resources/utils.xsl";
      File f = new File(sysid);
      StreamSource ret = new StreamSource(f);
      try {
      sysid = f.toURL().toExternalForm();
      }
      catch(Exception e) {
         throw new TransformerException("....");
      }
 
      ret.setSystemId(sysid);

      System.out.println("1 ret's system id is = " + ret.getSystemId());

      return ret;

/*
    String res = null; // (String)_uris.get(href);

    if( href.startsWith(pocoLoc) ) {
      res = href.replaceFirst(pocoLoc, "com/pocomatic/resources/");
    }

    if( res == null || ClassLoader.getSystemClassLoader().getResource(res) == null ) {
      return null;
    }

    try {
      System.out.println("resource = " + res);
      java.io.InputStream strm 
           = ClassLoader.getSystemClassLoader().getResourceAsStream(res);

      Source src = new StreamSource(strm);
      src.setSystemId("file://home/kejin/workspace/pococapsule-cpp/src/tools/com/pocomatic/resources/utils.xsl");

      System.out.println("returned resource opened at: " + res + " src = "+ src);
      //return src;
      String sysid = "file://home/kejin/workspace/pococapsule-cpp/src/tools/com/pocomatic/resources/utils.xsl";
      StreamSource ret = new StreamSource(new File(sysid));
      ret.setSystemId(sysid);

      System.out.println("ret's system id is = " + ret.getSystemId());

      return ret;
    }
    catch(Exception e) {
    }

    return null;
*/  }

  public static void add_uri(String href, String uri) {
    _uris.put(href, uri);
  }
};
