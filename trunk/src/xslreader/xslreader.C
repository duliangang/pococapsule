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

#include <jni.h>
#include <pocoapp.h>
#include <pocostr.h>
#include <stdlib.h>
#include <string.h>
#include "xslreader.h"
#include <signal.h>

#if defined(WIN32)
# include <windows.h>
  typedef HMODULE poco_dlhandle;
#  define POCO_DLHANDLE HMODULE
#  define POCO_DLOPEN(name) LoadLibrary(name)
#  define POCO_DLERROR "Fail to load"
#  define POCO_DLSYM(handle, sym_name) GetProcAddress(handle, sym_name)
#  define POCO_DLCLOSE(handle) FreeLibrary(handle)
#else
# include <dlfcn.h>
  typedef void * poco_dlhandle;
#  define POCO_DLOPEN(name) dlopen(name, RTLD_LAZY)
#  define POCO_DLSYM(handle, sym_name) dlsym(handle, sym_name)
#  define POCO_DLCLOSE(handle) dlclose(handle)
#  define POCO_DLERROR dlerror()
#endif

static JNIEnv* _env = NULL;
static jmethodID _mid1;
static jmethodID _mid2;
static jclass	_clz;
static JavaVM*	_jvm = NULL;

#if defined(WIN32)
# define CLNT_JVM "/jre/bin/client/jvm.dll"
# define CLZPATHDLIM	";"
#else
  // this is for Linux only. for other unix, it should be changed ...
# define CLNT_JVM "/jre/lib/i386/client/libjvm.so"
# define CLZPATHDLIM	":"
#endif

typedef jint (JNICALL *FuncPtrCreateJVM)(JavaVM **pvm, void **penv, void *args);

#include <sys/types.h>
#include <sys/stat.h>

static void stop_jvm()
{
	if( _jvm == NULL ) {
		return;
	}

	if(_env && _env->ExceptionOccurred()) {
		_env->ExceptionDescribe();
	}
	
	// _jvm->DestroyJavaVM();  // JVM seems couldn't be restart, bypass this. -Ke
	// _jvm = NULL;
}

static int start_jvm(POCO_AppEnv* pocoenv)
{
	if( _jvm != NULL ) {
		return 1;
	}

#if !defined(WIN32)
	sigset_t	_blksigs;
	sigprocmask(0, NULL, &_blksigs);
#endif	

	jint res = -1;

	JavaVMInitArgs vm_args;
	JavaVMOption options[1];
	POCO_String clzpath = (const char*)"-Djava.class.path=";
	clzpath += (const char*)getenv("CLASSPATH");
	if( strstr(clzpath.in(), "pocotools.jar") == NULL
	 && getenv("POCOCAPSULE_DIR") != NULL ) {
		clzpath += CLZPATHDLIM;
		clzpath += getenv("POCOCAPSULE_DIR");
		clzpath += "/lib/pocotools.jar";
	}
	options[0].optionString = clzpath.retn(); 
	vm_args.version = 0x00010004;
	vm_args.options = options;
	vm_args.nOptions = 1;
	vm_args.ignoreUnrecognized = JNI_TRUE;

	POCO_String path = (const char*)getenv("POCOCAPSULE_DIR");

	//
	// try bundled JVM first
	//
	if( path.in() != NULL ) {
		//path += "/jdk";
		path += CLNT_JVM;
	}
	struct stat buf;
	if( stat(path.in(), &buf) != 0 ) {
		//
		// then, try system installed JVM ...
		//
		path = (const char*)getenv("JAVA_HOME"); 

		if( path.in() != NULL ) {
		  path += CLNT_JVM;
		}
	}

	FuncPtrCreateJVM func = NULL;

	poco_dlhandle handle = POCO_DLOPEN(path.in());
	
	if (handle != NULL) {
		func = (FuncPtrCreateJVM)POCO_DLSYM(handle, "JNI_CreateJavaVM");
	}

	if (func == NULL ) {
		return 0;
	}

	res = func(&_jvm, (void**)&_env, &vm_args);
		
#if !defined(WIN32)
	sigprocmask(SIG_SETMASK, &_blksigs, NULL); // restore the original sigmasks
#endif	

	if (res < 0) {
		pocoenv->fatal_error("Can't create Java VM");
		return 0;
	}

	_clz = _env->FindClass("com/pocomatic/tools/Encode");
	if (_clz == NULL) {
		pocoenv->fatal_error("Can't find com.pocomatic.tools.Encode class");
		stop_jvm();
		return 0;
	}

	_mid1 = _env->GetStaticMethodID(_clz, "parse_xml_file", "(Ljava/lang/String;)Ljava/lang/String;");
	_mid2 = _env->GetStaticMethodID(_clz, "parse_xml_string", "(Ljava/lang/String;)Ljava/lang/String;");

	return 1;
}

static char* encode_xml(const char* xmlfile, int len, POCO_AppEnv* pocoenv)
{
	if( !start_jvm(pocoenv) ) {
		return NULL;
	}

	jstring jstr;

	if( len == 0 ) {
		jstr = (jstring)(_env->CallStaticObjectMethod(_clz, _mid1, _env->NewStringUTF(xmlfile)));
	}
	else {
		jstr = (jstring)(_env->CallStaticObjectMethod(_clz, _mid2, _env->NewStringUTF(xmlfile)));
	} 

	const char* s = _env->GetStringUTFChars(jstr, NULL); 

	POCO_String pstr(POCO_String::dup(s));
	_env->ReleaseStringUTFChars(jstr, s); 

	stop_jvm();

        if( strncmp(pstr.in(), "Error:", 6) == 0 ) {
		pocoenv->fatal_error(pstr.in());
		return NULL;
        }

	return pstr.retn();
}

POCO_DOM::Element* POCO_XSL_Reader::read(const char* file, POCO_AppEnv* env)
{
	POCO_String s(encode_xml(file, 0, env));

	if( s.in() == NULL ) {
		return NULL;
	}

	return this->POCO_DOM_Reader::read(s.in(), strlen(s.in()), env);
}

POCO_DOM::Element* POCO_XSL_Reader::read(const char* buf, int len, POCO_AppEnv* env)
{
	POCO_String s(encode_xml(buf, strlen(buf), env));

	if( s.in() == NULL ) {
		return NULL;
	}

	return this->POCO_DOM_Reader::read(s.in(), strlen(s.in()), env);
}

const char* POCO_XSL_Reader::tid() 
{
	return "xml";
}

static int retcode = pocomatic_register_desc_reader(new POCO_XSL_Reader);

