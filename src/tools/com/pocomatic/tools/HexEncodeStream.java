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

import java.io.OutputStream;

public class HexEncodeStream extends OutputStream {
  private final static String code = "0123456789abcdef";

  private OutputStream ostrm;
  private int count;
  private boolean header_done;

  HexEncodeStream(OutputStream strm) {
    ostrm = strm;
    count = 0;
    header_done = false;
  }
  
  public void write(int b) throws java.io.IOException {
    if( header_done == false ) {
      header_done = true;
      String header = "pococtx:";
      count = header.length();
      ostrm.write(header.getBytes(), 0, count);
    }

    int a = b%256;
    ostrm.write(code.charAt(a/16));
    ostrm.write(code.charAt(a%16));
    count += 2;
    if( count % 80 == 0 ) {
      ostrm.write('\n');
    } 
  }

  public void flush() throws java.io.IOException {
    ostrm.flush();
  }
};
