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
#include <pocostr.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#if defined(WIN32)
# include <windows.h>
  typedef HMODULE poco_dlhandle;
#  define POCO_DLHANDLE HMODULE
#  define POCO_DLOPEN(name) LoadLibrary(name)
#  define POCO_DLERROR "Fail to load JVM"
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

static void fatal_error(const char* msg, JNIEnv* env)
{
	if( msg ) {
		fprintf(stderr, "%s\n", msg);
	}

	if( env != NULL && env->ExceptionOccurred() ) {
		env->ExceptionDescribe();
	}

	exit(-1);
}

int poco_launch(int argc, char** argv, const char* clzName, const char* methodName)
{
	JNIEnv* env = NULL;
	jmethodID mid;
	jclass	clz;
	JavaVM*	jvm = NULL;

#if !defined(WIN32)
	sigset_t	blksigs;
	sigprocmask(0, NULL, &blksigs);
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
		fatal_error(POCO_DLERROR, env);
	}

	res = func(&jvm, (void**)&env, &vm_args);
		
#if !defined(WIN32)
	sigprocmask(SIG_SETMASK, &blksigs, NULL); // restore the original sigmasks
#endif	

	if (res < 0) {
		fatal_error(NULL, env);
	}

	clz = env->FindClass(clzName);
	if (clz == NULL) {
		fatal_error(NULL, env);
	}

	mid = env->GetStaticMethodID(clz, methodName, "([Ljava/lang/String;)V");

	if( mid == NULL ) {
		fatal_error(NULL, env);
	}

	jobjectArray jargs = env->NewObjectArray(argc, env->FindClass("java/lang/String"), env->NewStringUTF(""));
	for(int i=0;i<argc;i++) {
		env->SetObjectArrayElement(jargs, i, env->NewStringUTF((const char*)argv[i]));
	}

	env->CallStaticVoidMethod(clz, mid, jargs);

	if( env->ExceptionOccurred() ) {
		env->ExceptionDescribe();
		exit(-1);
	}

	return 0;
}
