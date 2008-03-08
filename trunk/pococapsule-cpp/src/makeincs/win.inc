#
# This file to be included by Makefile.inc on Windows defines common
# settings for Windows Platforms
#

# The Macros from Makefile.inc:
# VERSION
#

VER_RC_FILE = $(POCOCAPSULE_DIR)/src/resources/version.rc 
VER_RES_FILE = version.res

BIN_SUFFIX = .exe
DELIMITER = /
OBJ_EXT = obj

CC = CL.EXE
LINK = LINK $(LD_FLAGS)
LINK_DLL = LINK /DLL $(LD_FLAGS)
LINK_LIBS =
LIB_PREFIX =
CC_DEST_FLAG = /Fo$(REL_OBJ_DIR)
LD_DEST_FLAG = /OUT:
PLATFORM_CCFLAGS = /nologo \
                   /D_CRT_SECURE_NO_DEPRECATE /D_WINDOWS /DWIN32 \
                   /Od /FD /EHsc /GS /W3 /c /Fd$(REL_OBJ_DIR)/

PLATFORM_LDFLAGS = /nologo /SUBSYSTEM:CONSOLE /OPT:NOICF
CXX_SRC_FLAG = /Tp

ifeq ($(BUILD_TYPE), debug)
  DEBUG_CCFLAGS = /Zi /D_DEBUG
  LIB_SUFFIX =d.dll
  THREAD_CCFLAGS = /MDd
  PLATFORM_LDFLAGS += /DEBUG
else
  LIB_SUFFIX = .dll
  THREAD_CCFLAGS = /MD
endif

# Embedding manifest file to dll or exe when using VC8.0
LIB_POST_BUILD = mt.exe -manifest $(1).manifest "-outputresource:$(1);2"
BIN_POST_BUILD = mt.exe -manifest $(1).manifest "-outputresource:$(1);1"

#
# Install DLL and PDB (if DLL is a debug  version) to $(INSTALL_DIR)/bin
# and LIB file (if there is an import library) to $(INSTALL_DIR)/lib
# $(1) : Dynamic linked library   e.g. capsule.dll
#

define INSTALL_LIB
	$(call MKDIR_IF_NOT_EXIST,$(INSTALL_DIR)/lib)
	$(call MKDIR_IF_NOT_EXIST,$(INSTALL_DIR)/bin)
	@cd $(REL_OBJ_DIR); \
	echo ">Installing $(1) to $(INSTALL_DIR)/bin"; \
        cp -f $(1) $(INSTALL_DIR)/bin; \
	if [ "$(BUILD_TYPE)" == "debug" ]; then \
	  echo ">Installing $(1:.dll=.pdb) to $(INSTALL_DIR)/bin"; \
	  cp -f $(1:.dll=.pdb) $(INSTALL_DIR)/bin; \
	fi; \
	if [[ -f $(1:.dll=.lib) ]]; then \
	  echo ">Installing $(1:.dll=.lib) to $(INSTALL_DIR)/lib"; \
	  cp -f $(1:.dll=.lib) $(INSTALL_DIR)/lib; \
	fi;
endef

#
# Install EXE file and corresponding PDB file to $(INSTALL_DIR)/bin
#
# $(1) : EXE file   e.g. abin.exe
#

define INSTALL_BIN
	$(call MKDIR_IF_NOT_EXIST,$(INSTALL_DIR)/bin)
	@cd $(REL_OBJ_DIR); \
	echo ">Installing $(1) to $(INSTALL_DIR)/bin"; \
	cp -f $(1) $(INSTALL_DIR)/bin; \
	if [ "$(BUILD_TYPE)" == "debug" ]; then \
	  echo ">Installing $(1:.exe=.pdb) to $(INSTALL_DIR)/bin"; \
	  cp -f $(1:.exe=.pdb) $(INSTALL_DIR)/bin; \
	fi; 
endef

# 
# Compile windows resource file to add version information
#
# $(1): The file name to which the .res is to be linked
# $(2): The directory to put .res
# 
# VESION  : defined in $(POCOCAPSULE_DIR)/version.inc
# VER_RES_FILE : defined in this file 
# VER_RC_FILE  : defined in this file 
#
#

define COMPILE_WINRC
	rc /DPOCO_PRODUCT_NAME="\"$(POCO_PRODUCT_NAME)\"" \
	   /DPOCO_FILE_VERSION=$(VERSION:.=,) \
	   /DPOCO_PRODUCT_VERSION="\"$(VERSION)\"" \
	   /DPOCO_FILE_NAME="\"$(1)\"" \
	  /fo$(2)/$(VER_RES_FILE) $(VER_RC_FILE)
endef


#
#
# $(1): Library Name
define CREATE_LIBRARY
	@echo ">Creating $(1) in $(REL_OBJ_DIR)..."
	$(call COMPILE_WINRC,$(1),$(REL_OBJ_DIR))
	cd $(REL_OBJ_DIR); \
	$(LINK_DLL) $(LD_DEST_FLAG)$(1) $(OBJ_FILES) \
	            $(VER_RES_FILE) $(LIBPATH)
	$(call LIB_POST_BUILD,$(REL_OBJ_DIR)$(1))
endef
