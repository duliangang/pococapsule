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

public class CharEncodeStream extends OutputStream {
  private OutputStream ostrm;
  private int count;
  private boolean header_done;

  CharEncodeStream (OutputStream strm) {
    ostrm = strm;
    count = 0;
    header_done = false;
  }

  static final int EOD = 255;
  
  public void write(int b) throws java.io.IOException {
    if( header_done == false ) {
      header_done = true;
      String header = "\"pococtx:\\0";
      count = header.length() - 1;
      ostrm.write(header.getBytes(), 0, count + 1);
    }

    if( count == 0 ) {
      ostrm.write('"');
    }

    int a = b%256;
    if( a == '\"' || a == '\\' ) {
      ostrm.write('\\');
      ostrm.write(a);
      count += 2;
    }
    else
    if( a == '\n' ) {
      ostrm.write('\\');
      ostrm.write('n');
      count += 2;
    }
    else 
    if( a == '\0' ) {
      ostrm.write('\\');
      ostrm.write('0');
      count += 2;
    }
    else
    if( a == EOD ) {
      ostrm.write('"');
      ostrm.write('\n');
    }
    else {
      ostrm.write(a);
      count++;
    }

    if( count >= 78 ) {
      ostrm.write('"');
      ostrm.write('\n');
      count=0;
    }
  }

  public void flush() throws java.io.IOException {
    ostrm.flush();
  }

  public void close() throws java.io.IOException {
    flush();
    ostrm.close();
  }
};
