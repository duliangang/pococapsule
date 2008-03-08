.PHONY : check prebuild

CXX_EXT = C

all: check prebuild build install

SRC_FILES = xslreader.C

include ../makeincs/Makefile.inc
EXPORT_HEADERS =

ifeq ($(PLATFORM), Windows_NT)
  LINK_LIBS = /LIBPATH:../../../capsule/$(PLATFORM)/$(BUILD_TYPE) \
	      pococapsule$(LIB_SUFFIX:.dll=.lib)
else
  LINK_LIBS = 
endif



XSLREADER_LIB = $(LIB_PREFIX)pocoxsl$(LIB_SUFFIX)$(VERSION_SUFFIX)

CCINCLUDE_DIRS = . ../../include ../capsule $(JRE_INSTALL_DIR)/include

CCINCLUDES = $(addprefix $(DELIMITER)I, $(CCINCLUDE_DIRS))

ifeq ($(PLATFORM), Windows_NT)
  COMMON_DEFS = BUILD_POCOXSLREADER_DLL THREAD
  CCINCLUDES += $(DELIMITER)I$(JRE_INSTALL_DIR)/include/win32
else
  COMMON_DEFS = THREAD PTHREADS
  CCINCLUDES += $(DELIMITER)I$(JRE_INSTALL_DIR)/include/linux
endif


check::
	@echo Target check...
	$(CHECK_SETTINGS)

prebuild::
	@echo Target $@
	$(MAKE_DIRECTORIES)

build:: $(REL_OBJ_DIR)$(XSLREADER_LIB)

$(REL_OBJ_DIR)$(XSLREADER_LIB): $(OBJ_FILES)
	@echo Target $@
	$(call COMPILE_WINRC,$(XSLREADER_LIB),$(REL_OBJ_DIR))
	cd $(REL_OBJ_DIR); \
	$(LINK_DLL) $(LD_DEST_FLAG)$(XSLREADER_LIB) $(OBJ_FILES) \
	            $(VER_RES_FILE) $(LIBPATH)
	$(call LIB_POST_BUILD,$@)
	$(call CREATE_SOFT_LINK,$(REL_OBJ_DIR),$(XSLREADER_LIB),\
               $(XSLREADER_LIB:$(VERSION_SUFFIX)=))

clean:
	rm -fr $(REL_OBJ_DIR)

install:
	$(call INSTALL_LIB,$(XSLREADER_LIB))
	$(call CREATE_SOFT_LINK,$(INSTALL_DIR)/lib,$(XSLREADER_LIB),\
               $(XSLREADER_LIB:$(VERSION_SUFFIX)=))

