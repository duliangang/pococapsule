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

class MethodDesc {
  boolean		is_factory;
  boolean		is_typecast;
  boolean		is_injection;
  boolean		is_lifecycle;
  String		retv_class_ptr;
  String		retv_class;
  String		operation_name;
  String		this_class_ptr;
  String		this_class;
  ArgumentDesc[]	argumentsDescs;
  Element		the_elem;

  MethodDesc(Element elem) {
    is_factory = false;
    is_typecast = false;
    is_injection = false;
    is_lifecycle = false;
    retv_class_ptr = null;
    retv_class = null;
    operation_name = null;
    this_class_ptr = null;
    this_class = null;
    argumentsDescs = null;
    the_elem = elem;
  }

  public boolean equals(java.lang.Object o) {
    if( o == null ) {
	return false;
    }

    if( (o instanceof MethodDesc) == false ) {
        return false;
    }

    MethodDesc desc = (MethodDesc)o;

    return ( Util.compare(retv_class_ptr, desc.retv_class_ptr) 
          && Util.compare(operation_name, desc.operation_name)
          && Util.compare(this_class_ptr, desc.this_class_ptr)
          && Util.compare(argumentsDescs, desc.argumentsDescs));
  }

  public int hashCode() { return args_signature().hashCode(); }

  String args_signature() {
    String signature = "(";

    for(int i=0; argumentsDescs != null && i< argumentsDescs.length; i++) {
      if( i == 0 && argumentsDescs[i].is_index_arg ) {
        continue;
      }

      String type_name = argumentsDescs[i].get_type_name();

      if( argumentsDescs[i].is_array ) {
        signature += "int, " + type_name + "[]";
      }
      else {
        if( argumentsDescs[i].pass_syntax == ArgumentDesc.PASS_DEREF ) {
          if( type_name.endsWith("*") ) {
            type_name = Util.trimTrailingChar(type_name);
          }
          else {
            type_name = "*" + type_name;
          }
        }

        signature += type_name;
      }

      if( i+1 < argumentsDescs.length ) {
        signature += ", ";
      }
    }

    signature += ")";

    return signature;
  }
};
