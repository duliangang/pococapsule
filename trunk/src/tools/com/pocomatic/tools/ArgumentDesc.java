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

class ArgumentDesc {
  boolean	is_index_arg;
  boolean	is_array;
  boolean	is_bean;
  String	type;
  String	class_ptr;
  int		pass_syntax;

  public final static int PASS_VAL	= 0;
  public final static int PASS_PTR	= 0;
  public final static int PASS_DUP	= 2;
  public final static int PASS_DEREF	= 3;

  ArgumentDesc() {
    is_index_arg = false;
    is_array = false;
    is_bean = false;
    type = null;
    class_ptr = null;
    pass_syntax = PASS_PTR;
  }

  boolean equals(ArgumentDesc desc) {
    if( desc == null
     || is_index_arg != desc.is_index_arg
     || is_array != desc.is_array
     || is_bean != desc.is_bean 
     || pass_syntax != desc.pass_syntax ) {
     return false; 
    }

    return ( Util.compare(type, desc.type) 
          && Util.compare(class_ptr, desc.class_ptr) );   
  }
         
  String get_type_name() {
    if( type.equals("string") || type.equals("cstring") ) {
      return "const char*";
    }
    else
    if( type.equals("char") ) {
      return "char";
    }
    else
    if( type.equals("uchar") || type.equals("byte") ) {
      return "unsigned char";
    }
    else
    if( type.equals("short") ) {
      return "short";
    }
    else
    if( type.equals("ushort") ) {
      return "unsigned short";
    }
    else
    if( type.equals("int") ) {
      return "int";
    }
    else
    if( type.equals("uint") ) {
      return "unsigned";
    }
    else
    if( type.equals("long") ) {
      return "long";
    }
    else
    if( type.equals("ulong") ) {
      return "unsigned long";
    }
    else
    if( type.equals("float") ) {
      return "float";
    }
    else
    if( type.equals("double") ) {
      return "double";
    }
    else
    if( type.equals("namevalue") ) {
      return "POCO_NameValue";
    }

    return class_ptr;
  }

  String get_uval_name() {
    String type_name = get_type_name();

    if( is_bean == true || type.equals("bean") || type.equals("string") || type.equals("cstring") ) {
      return "(" + type_name + ")";
    }
    else {
      return "*(" + type_name + "*)";
    }
  }

  boolean is_namevalue() {
    return type.equals("namevalue");
  }
};
