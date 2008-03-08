
#
# This file to be included by Makefile.inc on Windows defines common
# settings for Windows Platforms
#

# The Macros from Makefile.inc:
# VERSION
#

ifeq ($(CPU),)
$(error CPU is not set)
endif

COMMON_DEFS += CPU=$(CPU) TOOL_FAMILY=gnu TOOL=gnu VXWORKS
TGT_DIR = $(WIND_BASE)/target

CCINCLUDES = -I$(TGT_DIR)/h -I$(TGT_DIR)/config/all -I$(TGT_DIR)/src/drv

PLATFORM_LDFLAGS = -r -nostdlib
# -r LD_PARTIAL_FLAGS
PIC =


ifeq ($(CPU),SIMNT)
  CC = ccsimpc
  NM = nmsimpc
  LINK = ldsimpc
  PLATFORM_CCFLAGS = -mpentium -ansi -O -fno-builtin -fno-defer-pop
# -fno-exceptions
endif

VER_RC_FILE = 
VER_RES_FILE = 

BIN_SUFFIX = 
DELIMITER = -
OBJ_EXT = o

LINK_DLL = $(LINK) 
LINK_LIBS =
LIB_PREFIX =
CC_DEST_FLAG = -o $(REL_OBJ_DIR)
LD_DEST_FLAG = -o 
                   
CXX_SRC_FLAG = 

LIB_SUFFIX = .so

ifeq ($(BUILD_TYPE), debug)
  DEBUG_CCFLAGS = 
else
  THREAD_CCFLAGS = 
endif

define INSTALL_LIB
	echo INSTALL_LIB
endef

define INSTALL_BIN
	echo INSTALL_BIN
endef

#
#
# $(1): Library Name
define CREATE_LIBRARY
	@echo "$(LINK) $(LD_FLAGS) $(OBJ_FILES) -o partialImage.o "
	@echo "$(NM) partialImage.o $(OBJ_FILES) |\
	wtxtcl $(WIND_BASE)/host/src/hutils/munch.tcl -c simpc >ctdt.c "
	@echo "$(LINK) $(LD_FLAGS) partialImage.o ctdt.o -o $(1)"
	@cd $(REL_OBJ_DIR); \
	$(LINK) $(LD_FLAGS) $(OBJ_FILES) -o partialImage.o ;\
	$(NM) partialImage.o $(OBJ_FILES) | wtxtcl \
		$(WIND_BASE)/host/src/hutils/munch.tcl -c simpc >ctdt.c ;\
	$(CC) $(CC_FLAGS) ctdt.c -o ctdt.o ;\
	$(LINK) $(LD_FLAGS) partialImage.o ctdt.o -o $(1)
endef
