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

import java.io.PrintStream;

class ProxyGen {
  private boolean _note;

  int generateProxies(PrintStream ostrm, MethodDesc[] descs) {
    if( descs == null ) {
      return 0;
    }
 
    int count = 0;
    count += genBeanFactoryProxies(ostrm, descs);
    count += genInjectionProxies(ostrm, descs);
    count += genLifecycleProxies(ostrm, descs);
    count += genTypecastProxies(ostrm, descs);

    if( count != descs.length ) {
      fatal_error("unknown");
    }

    return count;
  }

  private int genBeanFactoryProxies(PrintStream ostrm, MethodDesc[] descs) {
    int count = 0;
    ostrm.println("// ---------------------------------------\n"
                + "//  Proxies of constructors and factories \n"
                + "// ---------------------------------------");
    for(int i=0; i<descs.length; i++) {
      if( descs[i].is_factory ) {
        if( _note ) {
          ostrm.println("/****************************************");
          ostrm.println("   The element:\n"); 
          Util.printElemShallow(descs[i].the_elem, "     ", ostrm);
	  ostrm.println("\n   projects to the following proxy:");
	  ostrm.println("****************************************/");
        }
        ostrm.println(genProxy(descs[i], KIND_CTOR));
        count++;
      }
    }

    return count;
  }

  private int genInjectionProxies(PrintStream ostrm, MethodDesc[] descs) {
    int count = 0;
    ostrm.println("// -------------------------------------\n"
                + "//   Proxies of ioc methods on POCOs  \n"
                + "// -------------------------------------");
    for(int i=0; i<descs.length; i++) {
      if( descs[i].is_injection ) {
        if( _note ) {
          ostrm.println("/**************************************");
          ostrm.println("   The element:\n");
          Util.printElemShallow(descs[i].the_elem, "     ", ostrm);
	  ostrm.println("\n   projects to the following proxy:");
	  ostrm.println("***************************************/");
        }
        ostrm.println(genProxy(descs[i], KIND_SETTER));
        count++;
      }
    }

    return count;
  }

  private int genLifecycleProxies(PrintStream ostrm, MethodDesc[] descs) {
    int count = 0;
    ostrm.println("// -------------------------------------\n"
                + "//   Proxies of dup and destroy methods \n"
                + "// -------------------------------------");
    for(int i=0; i<descs.length; i++) {
      if( descs[i].is_lifecycle ) {
        ostrm.println(genProxy(descs[i], KIND_FACTORY));
        count++;
      }
    }
   
    return count;
  }

  private int genTypecastProxies(PrintStream ostrm, MethodDesc[] descs) {
    int count = 0;
    ostrm.println("// ------------------------------------\n"
                + "//   Proxies of POCO typecast methods  \n"
                + "// ------------------------------------");
    for(int i=0; i<descs.length; i++) {
      if( descs[i].is_typecast ) {
        ostrm.println(genProxy(descs[i], KIND_CAST));
        count++;
      }
    }

    return count;
  }

  public String body_indent = "    ";
  public String loop_indent = body_indent + "    ";
  public String parm_indent = body_indent + "            ";

  static final String proxy_header_str = "static void* ";
  static final String proxy_params_str
                = "(void* _poco_this, void* _poco_params[])";

  int proxy_count;

  ProxyGen(boolean a) { proxy_count = 0; _note = a; }

  private String genProxy(MethodDesc desc, int kind) {
    String vardef_bloc = "";
    String invoke_stmt = "";
    String return_stmt = "";

    String this_class_ptr = "";
    String retv_class_ptr = "";
    String invoker = "";

    boolean saw_array = false;
    boolean saw_param = false;

    // vardef_bloc += body_indent + "int _poco_i = 0, _poco_j = 0;\n";

    if( desc.retv_class_ptr == null ) {
      // void return
      invoke_stmt += body_indent;
      return_stmt += body_indent + "return (void*)0UL;\n";
    }
    else {
      // return a pointer
      vardef_bloc += body_indent + desc.retv_class_ptr + " _poco_var_retv;\n";
      invoke_stmt += body_indent + "_poco_var_retv = ";
      return_stmt += body_indent + "return (void*)_poco_var_retv;\n";
      retv_class_ptr = desc.retv_class_ptr;
    }

    if( desc.this_class_ptr != null ) {
      // not global, not class static method
      // cast "this" pointer to factory_bean_clz
      vardef_bloc += body_indent + desc.this_class_ptr + " _poco_var_this = ("
                   + desc.this_class_ptr + ")_poco_this;\n";
      invoke_stmt += "_poco_var_this->"; 

      //
      // 'invoker' string is used in generated comments
      //
      if( desc.this_class != null && desc.this_class.charAt(0) != '*' ) {
        invoker = desc.this_class + "::";
      }
      else {
        invoker = desc.this_class_ptr + "->";
      }

      this_class_ptr = desc.this_class_ptr;
    }

    String index_op_signature = "";

    if( desc.argumentsDescs == null || desc.argumentsDescs.length == 0 ) {
      invoke_stmt += desc.operation_name;
      if( desc.operation_name.charAt(desc.operation_name.length() - 1) != ';' ) {
        invoke_stmt += "();";
      }
    }
    else {
      String var_type_name;
      String var_uval_name;
      int i=0;
      int varcnt = 0;
      String deref = "";

      if( desc.argumentsDescs[0].is_index_arg ) {
        var_type_name = desc.argumentsDescs[i].get_type_name();
	var_uval_name = desc.argumentsDescs[i].get_uval_name();

        String var_name = "_poco_var_" + (varcnt++);

        vardef_bloc += body_indent + var_type_name + " " + var_name
                    + " = " + var_uval_name
                    + "(_poco_params[_poco_i++]);\n";

	saw_param = true;

        if( desc.argumentsDescs[0].pass_syntax == ArgumentDesc.PASS_DEREF ) {
          deref = "*";
        }

        invoke_stmt += "operator[](" + deref + var_name + ")";
        i=1;

        index_op_signature =  "operator[](" + var_type_name + ")";

	char c = desc.operation_name.charAt(0);

	if( Character.isUpperCase(c) 
	 || Character.isLowerCase(c) || c == '_' ) {
	  invoke_stmt += "->";
	  index_op_signature += "->";
	}
      }

      if( desc.is_typecast == false ) {
        //
        // No explicit typecast. for type safety.
        //
        invoke_stmt += desc.operation_name;
      }

      if( i == desc.argumentsDescs.length ) {
        invoke_stmt += "();";
      }
      else {
        saw_param = true;
        invoke_stmt += "(\n";

        for(;i<desc.argumentsDescs.length;i++) {
          var_type_name = desc.argumentsDescs[i].get_type_name();
          var_uval_name = desc.argumentsDescs[i].get_uval_name();
    
          if( desc.argumentsDescs[i].is_array ) {
            //
            // an array param
            //
            saw_array = true;
            String var_size_name  = "_poco_var_" + (varcnt++);
            String var_array_name = "_poco_var_" + (varcnt++);
    
            vardef_bloc += body_indent + "int " + var_size_name
                         + " = *((int*)(_poco_params[_poco_i++]));\n";

            vardef_bloc += body_indent + var_type_name + "* " 
                         + var_array_name + " = NULL;\n";
            vardef_bloc += body_indent + "if(" + var_size_name + ") {\n"
                         + loop_indent + var_array_name + " = new "
                         + var_type_name + "[" + var_size_name + "];\n"
                         + body_indent + "}\n"
                         + body_indent + "POCO_VAR " + var_array_name 
                         + "_var((char*)" + var_array_name + ");\n";
    
            vardef_bloc += body_indent + "for(_poco_j=0; _poco_j < "
                         + var_size_name + "; _poco_j++) {\n";
    
            if( desc.argumentsDescs[i].is_namevalue() ) {
              //
              // namevalue array
              //
              vardef_bloc += loop_indent 
                           + var_array_name + "[_poco_j].name  = "
                           + "(const char*)(_poco_params[_poco_i++]);\n";
              vardef_bloc += loop_indent
                           + var_array_name + "[_poco_j].value = "
                           + "(const char*)(_poco_params[_poco_i++]);\n";
            }
            else {
              //
              // not a namevalue array
              //
              vardef_bloc += loop_indent + var_array_name 
                           + "[_poco_j] = " + var_uval_name 
                           + "(_poco_params[_poco_i++]);\n";
            }
    
            vardef_bloc += body_indent + "}\n";
    
            invoke_stmt += parm_indent
                         + var_size_name + ", " + var_array_name;
          }
          else {
            //
            // non-array param
            //
            if( desc.argumentsDescs[i].pass_syntax == ArgumentDesc.PASS_DEREF ) {
              deref = "*";
            }
            else {
              deref = "";
            }

            String var_name = "_poco_var_" + (varcnt++);
  
            vardef_bloc += body_indent + var_type_name + " " + var_name
                         + " = " + var_uval_name
                         + "(_poco_params[_poco_i++]);\n";
    
            invoke_stmt += parm_indent + deref + var_name;
          }
    
          if( i+1 < desc.argumentsDescs.length ) {
            invoke_stmt += ",\n";
          }
        }

        invoke_stmt += ");";
      }
    }

    String idxdef_bloc = "";

    if( saw_array ) {
      idxdef_bloc = body_indent + "int _poco_i = 0, _poco_j = 0;\n";
    }
    else
    if( saw_param ) {
      idxdef_bloc = body_indent + "int _poco_i = 0;\n";
    }

    String signature = index_op_signature + desc.operation_name + desc.args_signature();

    String proxy_body = "//\n// " + invoker + signature + "\n//\n";

    proxy_body += proxy_header_str + "_poco_proxy_" + proxy_count 
                + proxy_params_str + "\n{\n"
		+ idxdef_bloc 
                + vardef_bloc + "\n" + invoke_stmt + "\n\n" 
                + return_stmt + "}\n\n"

                + "static pocomatic_register_proxy _poco_proxy_var_" + proxy_count + "(\n"
                + body_indent + kind + ",\n"
                + body_indent + "\"" + retv_class_ptr + "\",\n"
                + body_indent + "\"" + this_class_ptr + "\",\n"
                + body_indent + "\"" + signature + "\",\n"
                + body_indent + "_poco_proxy_" + proxy_count + ",\n" 
                + body_indent + "__FILE__,\n"
                + body_indent + "\"_poco_proxy_" + proxy_count + "\");\n";

    proxy_count++;

    return proxy_body;
  }

  void fatal_error(String msg) { }

  private static final int KIND_CTOR = 0;
  private static final int KIND_SETTER = 1;
  private static final int KIND_FACTORY = 2;
  private static final int KIND_CAST = 3;
}; 

      
