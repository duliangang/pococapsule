
#
# This file, which included by Makefile.inc on UNIX, defines common
# settings for UNIX Platforms
#

# The Macros from Makefile.inc:
# VERSION
#

ifneq ($(VERSION),)
  VERSION_SUFFIX = .$(VERSION)
endif

BIN_SUFFIX =
DELIMITER = -
OBJ_EXT = o
THREAD_CCFLGS = -mt
LIB_PREFIX = lib

CC_DEST_FLAG = -o $(REL_OBJ_DIR)
LD_DEST_FLAG = -o
PLATFORM_CCFLAGS = -c
PLATFORM_LDFLAGS =
ifeq ($(BUILD_TYPE), debug)
  BUILD_CCFLAGS = -g
endif

LIB_SUFFIX = .so

ifeq ($(PLATFORM), Linux)
  CC = g++
  PIC = -fpic
  ifeq ($(BUILD_TYPE), debug)
    DEBUG_CCFLAGS += -ggdb -Wall
  else
    DEBUG_CCFLAGS += -Os
  endif
  LINK = g++ $(LD_FLAGS)
  LINK_DLL = g++ -shared $(LD_FLAGS)
endif

ifneq ($(BUILD_TYPE), debug)
  LIB_POST_BUILD = strip $(1)
endif

#
# CREATE_SOFT_LINK 
# Create a soft link to a file
# takes three arguments:
# $(1) : Directory
# $(2) : File to be linked to
# $(3) : link
# 

ifneq ($(VERSION_SUFFIX),)
  define CREATE_SOFT_LINK
	@cd $(1); \
	if [ -f $(3) ]; then \
	  rm $(3); \
	fi; \
	ln -s $(2) $(3);
  endef
endif

#
# Install library file to $(INSTALL_DIR)/lib
# $(1) : Library to be installed
# 
# MKDIR_IF_NOT_EXIST
# REL_OBJ_DIR
# INSTALL_DIR
#
#

define INSTALL_LIB
	$(call MKDIR_IF_NOT_EXIST,$(INSTALL_DIR)/lib)
	@cd $(REL_OBJ_DIR); \
	echo ">Installing $(1) to $(INSTALL_DIR)/lib"; \
	cp -f $(1) $(INSTALL_DIR)/lib; 
	$(call CREATE_SOFT_LINK,$(INSTALL_DIR)/lib,$(1),\
	  $(1:$(VERSION_SUFFIX)=))
endef

#
# Install binary to installation directory
# $(1) :
# MKDIR_IF_NOT_EXIST
# REL_OBJ_DIR          
# INSTALL_DIR
#

define INSTALL_BIN
        $(call MKDIR_IF_NOT_EXIST,$(INSTALL_DIR)/bin)
        @cd $(REL_OBJ_DIR); \
        echo ">Installing $(1) to $(INSTALL_DIR)/bin"; \
        cp -f $(1) $(INSTALL_DIR)/bin; 
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
	$(call CREATE_SOFT_LINK,$(REL_OBJ_DIR),$(1),\
		$(1:$(VERSION_SUFFIX)=))
endef
