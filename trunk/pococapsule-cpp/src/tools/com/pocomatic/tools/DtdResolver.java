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

//
// todo: share the resolve of core?
//

public class DtdResolver implements org.xml.sax.EntityResolver {

  private static org.xml.sax.EntityResolver _next;
  private static java.util.Hashtable _doctypes;

  private static final String pocoLoc = "http://www.pocomatic.com/";

  static {
    _doctypes = new java.util.Hashtable();
    _doctypes.put(
		"http://www.pocomatic.com/poco-application-context.dtd", 
		"com/pocomatic/resources/poco-application-context.dtd");
    _doctypes.put(
		"http://www.pocomatic.com/corba-application-context.dtd",
		"com/pocomatic/resources/corba-application-context.dtd");
  }

  public org.xml.sax.InputSource resolveEntity(
                              String pubId, String sysId) {
    return resolve_resource(pubId, sysId);
  }

  static org.xml.sax.InputSource resolve_resource(String pubId, String sysId) {
    if( sysId == null ) {
      return null;
    }

    String res = (String)_doctypes.get(sysId);

    if( res == null ) {
      if( sysId.startsWith(pocoLoc) ) {
        res = sysId.replaceFirst(pocoLoc, "com/pocomatic/resources/");
      }
      else 
      if( sysId.startsWith("file:")) {
        res = sysId.substring("file:".length());
      }

      if( res == null || ClassLoader.getSystemClassLoader().getResource(res) == null ) {
        if( _next != null ) {
          try {
            return _next.resolveEntity(pubId, sysId);
          }
          catch(Exception e) { }
        }

        return null;
      }
    }

    try {
      java.io.InputStream strm 
            = ClassLoader.getSystemClassLoader().getResourceAsStream(res);
      org.xml.sax.InputSource src = new org.xml.sax.InputSource(strm);

      src.setPublicId(pubId);
      //src.setSystemId(sysId);
      src.setSystemId(ClassLoader.getSystemClassLoader().getResource(res).toExternalForm());

      return src;
    }
    catch(Exception e) {
    }

    return null;
  }

  static void setNext(org.xml.sax.EntityResolver next) {
    _next = next;
  }

  public static void add_doctype(String sysId, String doctype) {
    _doctypes.put(sysId, doctype);
  }
};
