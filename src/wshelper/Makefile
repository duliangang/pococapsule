.PHONY : check prebuild

CXX_EXT = C

all: check prebuild build install

SRC_FILES = wasphelper.C

include ../makeincs/Makefile.inc

ifeq ($(PLATFORM), Windows_NT)
  LINK_LIBS = /LIBPATH:../../../capsule/$(PLATFORM)/$(BUILD_TYPE)$(MODE) \
	      pococapsule$(LIB_SUFFIX:.dll=.lib) \
              /LIBPATH:$(WASPC_HOME)/bin WASPC.lib
else
  LINK_LIBS = -L$(WASPC_HOME)/lib -lwasp_stl 
endif


############## Define the library name

WSHELPER_LIB= $(LIB_PREFIX)poco2wasp$(LIB_SUFFIX)$(VERSION_SUFFIX)

CCINCLUDE_DIRS = ../../include $(WASPC_HOME)/include

CCINCLUDES = $(addprefix $(DELIMITER)I, $(CCINCLUDE_DIRS))

COMMON_DEFS = USE_WASP

ifeq ($(PLATFORM), Windows_NT)
  COMMON_DEFS += BUILD_WS_DLL
endif


check::
	@echo Target check...
	$(CHECK_SETTINGS)

prebuild::
	@echo Target $@
	$(MAKE_DIRECTORIES)

build:: $(REL_OBJ_DIR)$(WSHELPER_LIB)

$(REL_OBJ_DIR)$(WSHELPER_LIB): $(OBJ_FILES)
	@echo Target $@
	$(call COMPILE_WINRC,$(WSHELPER_LIB),$(REL_OBJ_DIR))
	cd $(REL_OBJ_DIR); \
	$(LINK_DLL) $(LD_DEST_FLAG)$(WSHELPER_LIB) $(OBJ_FILES) \
	            $(VER_RES_FILE) $(LIBPATH)
	$(call LIB_POST_BUILD,$@)
	$(call CREATE_SOFT_LINK,$(REL_OBJ_DIR),$(WSHELPER_LIB),\
               $(WSHELPER_LIB:$(VERSION_SUFFIX)=))

clean:
	rm -fr $(REL_OBJ_DIR)

install:
	$(call INSTALL_LIB,$(WSHELPER_LIB))
	$(call CREATE_SOFT_LINK,$(INSTALL_DIR)/lib,$(WSHELPER_LIB),\
               $(WSHELPER_LIB:$(VERSION_SUFFIX)=))
