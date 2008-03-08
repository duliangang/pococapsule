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
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import java.util.Set;
import java.util.Hashtable;
import java.util.ArrayList;

import java.io.*;
  
public class Parser {
  private static final String TOKEN_BEAN = "bean";
  private static final String TOKEN_ID = "id";
  private static final String TOKEN_NAME = "name";
  private static final String TOKEN_TYPE = "type";
  private static final String TOKEN_CLASS = "class";
  private static final String TOKEN_FACTORY_BEAN = "factory-bean";
  private static final String TOKEN_FACTORY_METHOD = "factory-method";
  private static final String TOKEN_DUP_METHOD = "dup-method";
  private static final String TOKEN_DESTROY_METHOD = "destroy-method";
  private static final String TOKEN_METHOD_ARG = "method-arg";
  private static final String TOKEN_ARRAY = "array";
  private static final String TOKEN_IOC = "ioc";
  private static final String TOKEN_ON_ERROR = "on-error";
  private static final String TOKEN_CAST = "cast";
  private static final String TOKEN_ITEM = "item";
  private static final String TOKEN_CONTEXT = "poco-application-context";
  private static final String TOKEN_REF = "ref";
  private static final String TOKEN_VALUE = "value";
  private static final String TOKEN_NULL = "null";
  private static final String TOKEN_LABEL = "label";
  private static final String TOKEN_ABSTRACT = "abstract";
  private static final String TOKEN_METHOD = "method";
  private static final String TOKEN_INDEX_ARG = "index-arg";
  private static final String TOKEN_PASS = "pass";
  private static final String TOKEN_PTR = "ptr";
  private static final String TOKEN_DEREF = "deref";
  private static final String TOKEN_DUP = "dup";
  private static final String TOKEN_DESTROY_BEAN = "destroy-bean";
  private static final String TOKEN_SELF_DESTROY_METHOD = "self-destroy-method";
  private static final String TOKEN_SELF_DUP_METHOD = "self-dup-method";
  private static final String TOKEN_HEADER = "header";
  private static final String TOKEN_IMPORT = "import";
  private static final String TOKEN_RESOURCE = "resource";
  private static final String TOKEN_THIS_BEAN = "this-bean";
  private static final String TOKEN_TARGET = "target";
  private static final String TOKEN_NONE = "none";

  public Parser() {
    descSet = new java.util.HashSet();
    ctxtSet = new java.util.HashSet();
    headerList = new java.util.ArrayList();
    classNameTable = new java.util.Hashtable();
    classPtrNameTable = new java.util.Hashtable();
  }

  public void parseDoc(Document doc) {
    classNameTable.clear();
    classPtrNameTable.clear();
    Element root = doc.getDocumentElement();

    String attr = root.getAttribute(TOKEN_ID);

    // todo
    //DescEncode.encodeElement(root, System.out);
    //System.out.write(255);
    //System.out.flush();
/*
    CharEncodeStream ostrm = new CharEncodeStream(System.out);
    PrintStream pstrm = new PrintStream(ostrm);
    DescEncode.encodeElement(root, pstrm);
    pstrm.write(CharEncodeStream.EOD);
    pstrm.flush();

    HexEncodeStream hstrm = new HexEncodeStream(System.out);
    pstrm = new PrintStream(hstrm);
    DescEncode.encodeElement(root, pstrm);
    pstrm.flush();

    try {
    ostrm.close();
    } catch(Exception e) {}
*/
    if( Util.is_empty(attr) == false  ) {
      if( ctxtSet.contains(attr) == true ) {
        System.out.println("duplicated context " + attr + " dropped");
        return;
      }
      else {
        ctxtSet.add(attr);
      }
    } 

    parseAllHeaders(root);
    parseAllBeans(root);
  }

  private void parseAllBeans(Element root) {
    NodeList nodeList = root.getChildNodes();

    parseIdToTypeMappings(root);

    for(int i=0; i<nodeList.getLength(); i++) {
      Node node = nodeList.item(i);

      if( node instanceof Element ) {
        Element elem = (Element)node;
        if( elem.getTagName().equals(TOKEN_BEAN) ) {
          parseBeanElement(elem, null);
        }
      }
    }
  }

  private void parseAllHeaders(Element root) {
    NodeList nodeList = root.getChildNodes();

    for(int i=0; i<nodeList.getLength(); i++) {
      Node node = nodeList.item(i);

      if( node instanceof Element ) {
        Element elem = (Element)node;

        if( elem.getTagName().equals(TOKEN_HEADER) ) {
          String text = Util.getTextContent(elem); 

          if( Util.is_empty(text) == false ) {
            headerList.add(text);
          }
        }
      }
    }
  }

  private void parseIdToTypeMappings(Element root) {
    NodeList nodeList = root.getElementsByTagName(TOKEN_BEAN);

    for(int i=0;i<nodeList.getLength();i++) {
      Element elem = (Element)nodeList.item(i);

      String beanId = Util.getTrimedAttribute(elem, TOKEN_ID);
      
      if( Util.is_empty(beanId) ) {
        continue;
      }

      String className = (Util.getTrimedAttribute(elem, TOKEN_CLASS)
                         .replace('{', '<').replace('}', '>'));
      String classPtrName = Util.class_to_ptr(className);

      if( Util.is_empty(classPtrName) ) {
        continue;
      }

      classNameTable.put(beanId, className);
      classPtrNameTable.put(beanId, classPtrName); 
    }
  }

  private String parseBeanElement(Element elem, String cast_target_ptr) {
    MethodDesc desc = new MethodDesc(elem);

    //
    // Get the bean class (or proxy return class). If the factory method
    // is not specified, this class will also be the constructor class.
    //
    desc.retv_class = (Util.getTrimedAttribute(elem, TOKEN_CLASS)
                      .replace('{', '<').replace('}', '>'));
    desc.retv_class_ptr = Util.class_to_ptr(desc.retv_class);

    if( Util.is_empty(desc.retv_class_ptr) && Util.is_empty(cast_target_ptr) == false ) {
      fatal_error("[P001] Can not cast a \"void\" bean to a non-void bean:", elem);
    }
      
    registerCastOperation(desc.retv_class_ptr, cast_target_ptr);

    String is_abstract = Util.getTrimedAttribute(elem, TOKEN_ABSTRACT);
    if( "true".equals(is_abstract) == false ) {
      //
      // get the factory bean type info 
      //
      String factoryBeanId = Util.getTrimedAttribute(elem, TOKEN_FACTORY_BEAN);
  
      if( Util.is_empty(factoryBeanId) ) {
        //
        // no factory bean, either ctor or a static call
        //
        desc.this_class = null;
        desc.this_class_ptr = null;
      }
      else {
        desc.this_class = resolve_bean_class(factoryBeanId);
        desc.this_class_ptr = resolve_bean_class_ptr(factoryBeanId);
      } 
  
      //
      // get factory method.
      //
      String factoryMethod = Util.getTrimedAttribute(elem, TOKEN_FACTORY_METHOD).replace('{', '<').replace('}', '>');
  
      if( Util.is_empty(factoryMethod) ) {
        //
        // no factory method, should be a ctor
        //
        if( desc.retv_class.charAt(0) == '*' ) {
          fatal_error("[P002] The \"factory-method\" attribute is missing:", elem);
        }
 	else 
        if( Util.is_empty(desc.retv_class_ptr) == false ) { 
          //
          // constructor factory
          //
          desc.is_factory = true;
          desc.operation_name = "new " + desc.retv_class;
        }
      }
      else {
        //
        // factory
        //
        desc.is_factory = true;
        desc.operation_name = factoryMethod;
      }
  
      desc.argumentsDescs = parseMethodArgs(elem, null);
      registerMethod(desc);
    }

    parseReleaseMethod(elem, desc);

    if( desc.retv_class_ptr != null ) {
      parseDupMethod(elem, desc);
      // parseReleaseMethod(elem, desc);
    }

    parseBeanProperties(elem, desc);

    // all other by-line beans 
    NodeList nodeList = elem.getChildNodes();

    for(int i=0; i<nodeList.getLength(); i++) {
      Node node = nodeList.item(i);

      if( node instanceof Element ) {
        elem = (Element)node;
        if( elem.getTagName().equals(TOKEN_BEAN) ) {
          parseBeanElement(elem, null);
        }
      }
    }

    return desc.retv_class_ptr;
  }

  private ArgumentDesc[] parseMethodArgs(Element elem, MethodDesc this_bean) {
    int size = Util.numberOfChildOfTagName(elem, TOKEN_METHOD_ARG);
    size += Util.numberOfChildOfTagName(elem, TOKEN_INDEX_ARG);

    if( size == 0 ) {
      return null;
    }

    ArgumentDesc[] args = new ArgumentDesc[size];
    NodeList nodeList = elem.getChildNodes();

    for(int i=0, count=0; count<size; i++) {
      Node node = nodeList.item(i);

      if( node instanceof Element ) {
        elem = (Element)node;
        if( elem.getTagName().equals(TOKEN_METHOD_ARG) 
	 || elem.getTagName().equals(TOKEN_INDEX_ARG) ) {
          args[count++] = parseParam(elem, this_bean);
        }
      }
    }

    return args;
  }

  private ArgumentDesc parseParam(Element elem, MethodDesc this_bean) {
    ArgumentDesc arg = new ArgumentDesc();

    arg.type = Util.getTrimedAttribute(elem, TOKEN_TYPE);
    if( Util.is_empty(arg.type) ) {
      arg.type = TOKEN_BEAN; // default is "bean"
      // fatal_error("[P003] The \"type\" attribute is missing:", elem);
    }

    arg.is_index_arg = elem.getTagName().equals(TOKEN_INDEX_ARG);
    arg.is_array = arg.type.equals(TOKEN_ARRAY);
    arg.is_bean = arg.type.equals(TOKEN_BEAN);
    Element belem = null;

    if( arg.is_bean ) {
      String attr = Util.getTrimedAttribute(elem, TOKEN_CLASS)
                    .replace('{', '<').replace('}', '>');

      arg.class_ptr = Util.class_to_ptr(attr);

      if( Util.is_empty(attr) == false && Util.is_empty(arg.class_ptr) ) {
        fatal_error("[P004] The value of \"class\" attribute is invalid:", elem);
      }

      attr = Util.getTrimedAttribute(elem, TOKEN_PASS);

      if( Util.is_empty(attr) ) {
        attr = TOKEN_PTR;
      }

      if( attr.equals(TOKEN_PTR) ) {
        arg.pass_syntax = ArgumentDesc.PASS_PTR;
      }
      else
      if( attr.equals(TOKEN_DUP) ) {
        arg.pass_syntax = ArgumentDesc.PASS_DUP;
      }
      else
      if( attr.equals(TOKEN_DEREF) ) {
        arg.pass_syntax = ArgumentDesc.PASS_DEREF;
      }

      attr = parseBeanParam(elem, arg.class_ptr, this_bean);

      if( Util.is_empty(arg.class_ptr) ) {
        if( Util.is_empty(attr) ) {
          fatal_error("[P004] The \"class\" attribute is missing:", elem);
        }

        arg.class_ptr = attr;
      }
    }
    else 
    if( arg.is_array ) {
      // if type is array, use the <array> element's type and class
      belem = Util.getFirstChildOfTagName(elem, TOKEN_ARRAY);
      if( belem == null ) {
        fatal_error("[P005] The <array> child element is missing:", elem);
      }

      arg.type = Util.getTrimedAttribute(belem, TOKEN_TYPE);
      if(Util.is_empty(arg.type)) {
        arg.type = TOKEN_BEAN;
      }
      arg.class_ptr = Util.class_to_ptr(
                  (Util.getTrimedAttribute(belem, TOKEN_CLASS)
                  .replace('{', '<').replace('}', '>')));

      if( Util.is_empty(arg.type) ) {
        fatal_error("[P006] The array's \"type\" attribute is missing:", elem);
      }

      if( arg.type.equals(TOKEN_BEAN) ) {
        if( Util.is_empty(arg.class_ptr) ) {
          fatal_error("[P007] The array's \"class\" attribute is missing:", elem);
        }

        // parse Array of Beans to generate needed
        // casting operations.
        parseArrayOfBean(belem, this_bean);
      }
    }

    return arg;
  }

  private String parseBeanParam(Element elem, String cast_target_ptr, MethodDesc this_bean) {
    // look at the ref short-cut attribute
    String ref = Util.getTrimedAttribute(elem, TOKEN_REF);
    Element belem = null;

    if( Util.is_empty(ref) ) {
      // no short cut, look at the bean attribute of a <ref> 
      // child element
      belem = Util.getFirstChildOfTagName(elem, TOKEN_REF);
      if( belem != null ) {
        ref = Util.getTrimedAttribute(belem, TOKEN_BEAN);
      }
    }

    if( Util.is_empty(ref) ) {
      if( elem.getFirstChild() == null ) {
        // no child node, == null ref;
        return null;
      }

      // look at the <bean> child element
      belem = Util.getFirstChildOfTagName(elem, TOKEN_BEAN);

      if( belem == null ) {
        belem = Util.getFirstChildOfTagName(elem, TOKEN_THIS_BEAN);

        if( belem != null ) {
          if( this_bean == null ) {
            fatal_error("[P008] Invalide use of this-bean argument", elem);
          }

          registerCastOperation(this_bean.retv_class_ptr, cast_target_ptr);
          return this_bean.retv_class_ptr;
        }
      }

      if( belem == null ) {
        belem = Util.getFirstChildOfTagName(elem, TOKEN_NULL);

        if( belem == null ) {
          fatal_error("[P008] Unknown:", elem);
        }

        return null;
      }

      // recursive
      return parseBeanElement(belem, cast_target_ptr);
    }
    else {
      // using the id of the reference 
      String class_ptr = resolve_bean_class_ptr(ref);

      if( Util.is_empty(class_ptr) ) {
        fatal_error("[P009] The \"class\" attribute is missing:", elem);
      }

      registerCastOperation(class_ptr, cast_target_ptr);

      return class_ptr;
    }
  }

  private void parseArrayOfBean(Element array_elem, MethodDesc this_bean) {
    NodeList nodeList = array_elem.getChildNodes();
    String class_ptr = Util.class_to_ptr(
		Util.getTrimedAttribute(array_elem, TOKEN_CLASS)
                .replace('{', '<').replace('}', '>'));

    for(int i=0; i<nodeList.getLength(); i++) {
      Node node = nodeList.item(i);

      if( node instanceof Element ) {
        Element elem = (Element)node;
        if( elem.getTagName().equals(TOKEN_ITEM) ) {
          parseBeanParam(elem, class_ptr, this_bean);
        }
      }
    }
  }

  void registerCastOperation(String class_ptr, String cast_target_ptr) {
    if( Util.is_empty(class_ptr)
     || Util.is_empty(cast_target_ptr)
     || class_ptr.equals(cast_target_ptr) ) {
      return;
    }

    //
    // <cast_target_ptr> (<cast_target_ptr>)(<class_ptr>)
    //   

    MethodDesc desc = new MethodDesc(null);

    desc.is_typecast = true;
    desc.retv_class_ptr = cast_target_ptr;
    desc.operation_name = "(" + cast_target_ptr + ")";
    ArgumentDesc arg = new ArgumentDesc();
    arg.is_bean = true;
    arg.type = TOKEN_BEAN;
    arg.class_ptr = class_ptr;
    desc.argumentsDescs = new ArgumentDesc[]{arg};

    registerMethod(desc);
  }

  //
  // register the dup-method, if exists.
  //
  void parseDupMethod(Element elem, MethodDesc rdesc) {
    if( Util.is_empty(rdesc.retv_class_ptr) ) {
      return;
    }

    boolean self_dup = true;

    String methodName = Util.getTrimedAttribute(elem, TOKEN_SELF_DUP_METHOD)
				.replace('{', '<').replace('}', '>');

    if( Util.is_empty(methodName) ) {
      self_dup = false;
      methodName = Util.getTrimedAttribute(elem, TOKEN_DUP_METHOD)
				.replace('{', '<').replace('}', '>');

      if( Util.is_empty(methodName) ) {
        return;
      }
    }

    //
    // <class_ptr> <dup-method>(<class_ptr>)
    //
    MethodDesc desc = new MethodDesc(null);
    desc.is_lifecycle = true;
    desc.operation_name = methodName;

    if( self_dup ) {
      //
      // void self-><dup-method>()
      // 
      desc.this_class_ptr = rdesc.retv_class_ptr;
      desc.this_class     = rdesc.retv_class;
    }
    else {
      //
      // <bean-class-ptr> <dup-method>(<bean-class-ptr>)
      //
      desc.retv_class_ptr = rdesc.retv_class_ptr;
      desc.retv_class     = rdesc.retv_class; 
    
      ArgumentDesc arg = new ArgumentDesc();
      arg.is_bean = true;
      arg.type = TOKEN_BEAN;
      arg.class_ptr = rdesc.retv_class_ptr;

      desc.argumentsDescs = new ArgumentDesc[]{arg};
    }

    registerMethod(desc);
  }

  //
  // register the release-method, if exists
  //
  void parseReleaseMethod(Element elem, MethodDesc rdesc) {
    boolean is_void_bean = Util.is_empty(rdesc.retv_class_ptr);

    boolean self_destroy = true;
    String methodName = Util.getTrimedAttribute(elem, TOKEN_SELF_DESTROY_METHOD)
				.replace('{', '<').replace('}', '>');

    if( Util.is_empty(methodName) ) {
      self_destroy = false;
      methodName =  Util.getTrimedAttribute(elem, TOKEN_DESTROY_METHOD)
                                .replace('{', '<').replace('}', '>');

      if( Util.is_empty(methodName) ) {
        return;
      }
    }

    //
    // <release-method>(<class_ptr>)
    //
    MethodDesc desc = new MethodDesc(null);
    desc.is_lifecycle = true;
    desc.retv_class_ptr = null;
    desc.operation_name = methodName;

    String releaseBeanId = Util.getTrimedAttribute(elem, TOKEN_DESTROY_BEAN);
    String thisBeanId    = Util.getTrimedAttribute(elem, TOKEN_ID);

    if( Util.is_empty(thisBeanId) == false && thisBeanId.equals(releaseBeanId) ) {
       self_destroy = true;
    }

    if( self_destroy ) {
      if( is_void_bean ) {
        fatal_error("[P010] invalid destroy declaration for a void bean", elem);
      }

      //
      // self->release();
      //
      desc.this_class = rdesc.retv_class;
      desc.this_class_ptr = rdesc.retv_class_ptr;
    }
    else 
    if( Util.is_empty(releaseBeanId) ) {
      //
      // no release bean, implies static call release(this_bean);
      //
      if( is_void_bean == false ) {
        ArgumentDesc arg = new ArgumentDesc();
        arg.is_bean = true;
        arg.type = TOKEN_BEAN;
        arg.class_ptr = rdesc.retv_class_ptr;
        desc.argumentsDescs = new ArgumentDesc[]{arg};
      }
    }
    else {
      //
      // release_bean->release(this_bean);
      //
      desc.this_class = resolve_bean_class(releaseBeanId);
      desc.this_class_ptr = resolve_bean_class_ptr(releaseBeanId);

      if( is_void_bean == false ) {
        ArgumentDesc arg = new ArgumentDesc();
        arg.is_bean = true;
        arg.type = TOKEN_BEAN;
        arg.class_ptr = rdesc.retv_class_ptr;

        desc.argumentsDescs = new ArgumentDesc[]{arg};
      }
    }

    registerMethod(desc);
  }

  void parseBeanProperties(Element bean_elem, MethodDesc desc) {
    NodeList nodeList = bean_elem.getChildNodes();

    for(int i=0; i<nodeList.getLength(); i++) {
      Node node = nodeList.item(i);

      if( node instanceof Element ) {
        Element elem = (Element)node;
        if( elem.getTagName().equals(TOKEN_IOC) ) {
          parseIoCElement(elem, desc);
        }
        else
        if( elem.getTagName().equals(TOKEN_ON_ERROR) ) {
          parseOnError(elem, desc);
	}
      }
    }
  }

  void parseIoCElement(Element elem, MethodDesc rdesc) {
    MethodDesc desc = new MethodDesc(elem);

    desc.is_injection = true;
    desc.this_class_ptr = null;
    desc.this_class = null;
    desc.operation_name = Util.getTrimedAttribute(elem, TOKEN_METHOD).replace('{', '<').replace('}', '>');

    if( Util.is_empty(rdesc.retv_class_ptr) == false ) {
      // a non-void bean
      String attr = elem.getAttribute(TOKEN_TARGET);

      if( Util.is_empty(attr) || attr.equals(TOKEN_THIS_BEAN) ) {
        desc.this_class_ptr = rdesc.retv_class_ptr;
        desc.this_class = rdesc.retv_class;
      }
      else
      if( attr.equals(TOKEN_NONE) == false ) {
        fatal_error("invalide target attribute value", elem);
      }
    }

    if( Util.is_empty(desc.operation_name) ) {
      fatal_error("[P011] The ioc method is missing:", elem);
    }

    desc.argumentsDescs = parseMethodArgs(elem, rdesc);
    registerMethod(desc);
  }

  void parseOnError(Element elem, MethodDesc rdesc) {
    MethodDesc desc = new MethodDesc(elem);

    desc.is_injection = true;
    desc.this_class_ptr = null;
    desc.this_class = null;
    desc.operation_name = Util.getTrimedAttribute(elem, TOKEN_METHOD).replace('{', '<').replace('}', '>');

    if( Util.is_empty(desc.operation_name) ) {
      fatal_error("[P011] The on-error method is missing:", elem);
    }

    desc.argumentsDescs = parseMethodArgs(elem, rdesc);
    registerMethod(desc);
  }

  public static String argsToSignature(String Oper, ArgumentDesc[] args) {
    String signature = Oper + "(";

    for(int i=0; args != null && i< args.length; i++) {
      String type_name = args[i].get_type_name();

      if( args[i].is_array ) {
        signature += "int, " + type_name + "[]";
      }
      else {
        signature += type_name;
      }

      if( i+1 < args.length ) {
        signature += ", ";
      }
    }

    signature += ")";

    return signature;
  }

  public boolean contains_context(String id) {
    if( Util.is_empty(id) ) {
      return false;
    }

    return ctxtSet.contains(id);
  }

  public String get_header_text(int i) {
    if( i >= headerList.size() ) {
      return null;
    }

    String text = null;

    try {
      text = (String)headerList.get(i);
    }
    catch(Exception e) {
    }

    return text;
  }

  private Set ctxtSet;
  private Set descSet;
  private ArrayList headerList;
  private Hashtable classNameTable;
  private Hashtable classPtrNameTable;

  private void registerMethod(MethodDesc desc) { 
    descSet.add(desc);
  }

  public MethodDesc[] allMethods() {
    int size = descSet.size();

    if( size == 0 ) {
      return null;
    }

    MethodDesc[] ret = new MethodDesc[size];
    Object[] arr = descSet.toArray();

    for(int i=0; i<size; i++) {
      ret[i] = (MethodDesc)(arr[i]);
    }

    return ret;
  }

  private String resolve_bean_class(String id) {
    return (String)classNameTable.get(id);
  }

  private String resolve_bean_class_ptr(String id) {
    return (String)classPtrNameTable.get(id);
  }

  void fatal_error(String msg, Element elem) {
    Node node = elem;
    String label = null;
    String id = null;

    for(node=elem;node!=null;node=node.getParentNode()) { 
      if( node instanceof Element ) {
        label = ((Element)node).getAttribute(TOKEN_LABEL);
        id = ((Element)node).getAttribute(TOKEN_ID);
      }

      if( Util.is_empty(label) == false
       || Util.is_empty(id) == false ) { 
        break;
      }
    }

    if( Util.is_empty(label) && Util.is_empty(id) ) {  
      msg += " at the <" + elem.getTagName() + "...>";
    }
    else {
      String loc = null;

      if( Util.is_empty(id) == false ) {
        loc = "id = \"" + id + "\"";
      }
      else {
        loc = "label = \"" + label + "\"";
      }

      if( node != null && node.equals(elem) == false ) {
        msg += " at the <" + elem.getTagName() + " ...>" 
             + " under the <" + ((Element)node).getTagName() 
             + " ... " + loc + " ...>";
      }
      else {
        msg += " at the <" + elem.getTagName() 
             + "... " + loc + " ...>";
      }
    }

    Exception e = new Exception(msg);
    e.printStackTrace();
  }  
};
