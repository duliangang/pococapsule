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
/*  VxWorks and Windows: xtli <xtli@pocomatic.com>                          */
/*									    */
/****************************************************************************/

#include <libloader.h>

#if !defined(VXWORKS)
#  if defined(WIN32)
#    include <windows.h>
#  else
#    include <dlfcn.h>
#  endif
#else
#include <unistd.h>
#include <fcntl.h>
#include <vxWorks.h>
#  include <loadLib.h> // for module load
#  include <unldLib.h> // for module unload
#  include <symLib.h> //for findSymbol
#  include <sysSymTbl.h> // for global symbol table
#include <usrLib.h>
#endif

#  include <stdlib.h>

#if defined(WIN32)
#  define POCO_DLOPEN(name) LoadLibrary(name)
#  define POCO_DLERROR "Fail to load"
#  define POCO_DLSYM(handle, sym_name) GetProcAddress(handle, sym_name)
#  define POCO_DLCLOSE(handle) FreeLibrary(handle)
#  if defined(_DEBUG)
#    define DLL_EXT		"d.dll"
#  else
#    define DLL_EXT		".dll"
#  endif
#  define DLL_SFX		"\0"
#elif defined(VXWORKS)
#  define POCO_DLOPEN(name) poco_dlopen(name)
#  define POCO_DLERROR "Fail to load"
#  define DLL_EXT		".so"
#  define DLL_SFX		"lib"
#else
#  define POCO_DLOPEN(name) dlopen(name, RTLD_LAZY|RTLD_GLOBAL)
#  define POCO_DLSYM(handle, sym_name) dlsym(handle, sym_name)
#  define POCO_DLCLOSE(handle) dlclose(handle)
#  define POCO_DLERROR dlerror()
#  define DLL_EXT		".so"
#  define DLL_SFX		"lib"
#endif

#include "pocostr.h"
#include <string.h>
#include <stdio.h>

#if defined(VXWORKS)
extern SYMTAB_ID sysSymTbl;
FUNCPTR taskEntry = NULL;
SYM_TYPE * pType;
typedef void (*POCO_InitFunPtr)();

MODULE_ID poco_dlopen(const char *name)
{
  if ( name[0] == '.' && name[1] == '/') {
    name +=2;
  }
//  printf("open(%s)\n",name);
  int fd = open(name, O_RDONLY, 0);
  if (fd == -1 ) {
  printf("failed to open(%s)\n",name);
    return NULL;
  }
  MODULE_ID handle = loadModule (fd,
    LOAD_ALL_SYMBOLS|LOAD_COMMON_MATCH_ALL);
  if (!handle ) {
    printf("failed to load module\n");
  }
  printf("handle = %X\n", handle);
  close(fd);
  return handle;
}

#endif
static char* replacesubstring(const char* orig, const char* oldsub, const char* newsub)
{
	POCO_String	ret = orig;
	char* buf = ret.inout();

	char* ptr = (char *)strstr(ret.in(), oldsub);

	if( ptr == NULL ) {
		return ret.retn();
	}

	ptr[0] = 0;
	ret += newsub;
	ret += (const char*)(orig + (ptr - buf) + strlen(oldsub));

	return replacesubstring(ret.in(), oldsub, newsub);
}

int POCO_LibLoader::load(
	const char*	lib,
	POCO_AppEnv*	env)
{
	POCO_String	path(lib);

	const char* ptr = strstr(lib, ".$dll");
	if( ptr != NULL && (strlen(ptr) == strlen(".$dll")) ) {
		char* buf = path.inout();
		buf[strlen(lib) - strlen(".$dll")] = 0;
		path += DLL_EXT;
	}

	path = replacesubstring(path.in(), ".$(dll)", DLL_EXT);
	path = replacesubstring(path.in(), ".${dll)", DLL_EXT);
	path = replacesubstring(path.in(), "$(lib)", DLL_SFX);
	path = replacesubstring(path.in(), "${lib}", DLL_SFX);
	lib = path.in();

	if( POCO_DLOPEN(lib) == NULL ) {
		env->fatal_error(POCO_DLERROR);
		return 0;
	}

	return 1;
}

#if 0
POCO_LibInit::POCO_LibInit(POCO_FunPtr_LibInit fun)
{
 (*fun)();
}
#endif
