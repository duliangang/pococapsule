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

import org.xml.sax.SAXParseException;
import org.xml.sax.ErrorHandler;

class ErrorHandlerImpl implements org.xml.sax.ErrorHandler {
  private String filename;

  ErrorHandlerImpl(String name) { filename = name; }

  public void warning(SAXParseException e) throws org.xml.sax.SAXException {
    printExceptionMsg("Warning", e);
  }

  public void error(SAXParseException e) throws org.xml.sax.SAXException {
    printExceptionMsg("Error", e);
  }

  public void fatalError(SAXParseException e)  throws org.xml.sax.SAXException {
    printExceptionMsg("Fatal Error", e);
  }

  private void printExceptionMsg(String level, SAXParseException e) {
    System.err.println(level + 
	": in file " + filename + " (or its schema), at " +
	"line "   + e.getLineNumber() + " and " +
	"column " + e.getColumnNumber() + ", " + 
	e.getMessage());
  }
};
