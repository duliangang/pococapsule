.SUFFIXES:
.SUFFIXES: .C .o .h .obj

ifndef POCOCAPSULE_DIR
$(error environment variable POCOCAPSULE_DIR is not set)
endif

include $(POCOCAPSULE_DIR)/src/makeincs/version.inc

ifeq ($(PLATFORM),)
  ifeq ($(OS),)
    PLATFORM=$(shell uname -s)
  else
    PLATFORM=Windows_NT
  endif
endif

SHELL = /bin/sh

ifndef BUILD_TYPE
    BUILD_TYPE=debug
endif

#
# Take platform specific settings by including win.inc or unix.inc
#

ifeq ($(PLATFORM),Windows_NT)
  include $(POCOCAPSULE_DIR)/src/makeincs/win.inc
else
  ifeq ($(PLATFORM),VxWorks)
    include $(POCOCAPSULE_DIR)/src/makeincs/vxworks.inc
  else
    include $(POCOCAPSULE_DIR)/src/makeincs/unix.inc
  endif
endif

CXX_EXT = C

#
#  DELIMITER
#  OBJ_EXT
#  CC
#  LINK
#  CC_DEST_FLAG
#  REL_OBJ_DIR
#  COMMON_DEFS
#
#  LINK_LIBS =
#  LIB_PREFIX =

OBJ_DIR_TAG =
REL_OBJ_DIR = ./$(PLATFORM)/$(OBJ_DIR_TAG)$(BUILD_TYPE)$(MODE)/
CC_MACROS = $(addprefix $(DELIMITER)D, $(COMMON_DEFS))
INSTALL_DIR = $(POCOCAPSULE_DIR)

VPATH = $(REL_OBJ_DIR)

OBJ_FILES = $(SRC_FILES:.$(CXX_EXT)=.$(OBJ_EXT)) 

LIBPATH = $(LINK_LIBS)
CC_FLAGS = $(PLATFORM_CCFLAGS) $(DEBUG_CCFLAGS) $(THREAD_CCFLAGS) $(PIC) \
           $(CC_MACROS) $(CCINCLUDES)

LD_FLAGS = $(PLATFORM_LDFLAGS)

define MAKE_DIRECTORIES
	@echo
	@echo
	@echo Preparing the directory structure for a build ...
	@echo
	$(shell mkdir -p $(REL_OBJ_DIR))
	@echo
	@echo Building ...
	@echo
endef

define CHECK_SETTINGS
	@echo PLATFORM=$(PLATFORM)
	@echo CC=$(CC)
	@echo CC_FLAGS=$(CC_FLAGS)
	@echo
	@echo CC Command line:
	@echo "$(CC) $(CC_FLAGS) $(CC_DEST_FLAG)<object file> $(CXX_SRC_FLAG) <source file>"
	@echo
endef

define MKDIR_IF_NOT_EXIST
	@if [ ! -d $(1) ]; then \
	  echo "mkdir -p $(1)"; \
          mkdir -p $(1); \
        fi	
endef

%.${OBJ_EXT}:%.${CXX_EXT}
	@echo "> Compiling $< ..."
	@$(CC) $(CC_FLAGS) $(CC_DEST_FLAG)$@ $(CXX_SRC_FLAG) $<

