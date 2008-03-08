.PHONY : check prebuild

CXX_EXT = C

all: check prebuild build install

SRC_FILES = \
dtd.C \
pocodtdres.C \
xmlreader.C

include ../makeincs/Makefile.inc
EXPORT_HEADERS =


ifeq ($(PLATFORM), Windows_NT)
  LINK_LIBS = /LIBPATH:$(XERCES_DIR)/lib xerces-c_2$(LIB_SUFFIX:.dll=.lib) \
	      /LIBPATH:../../../capsule/$(PLATFORM)/$(BUILD_TYPE) \
	      pococapsule$(LIB_SUFFIX:.dll=.lib)
else
  LINK_LIBS = -L$(XERCES_DIR)/lib -lxerces-c -lpthread
endif



XMLREADER_LIB = $(LIB_PREFIX)pocoxml$(LIB_SUFFIX)$(VERSION_SUFFIX)
CCINCLUDES = $(DELIMITER)I. $(DELIMITER)I../../include $(DELIMITER)I../capsule $(DELIMITER)I$(XERCES_DIR)/include

ifeq ($(PLATFORM), Windows_NT)
  COMMON_DEFS = BUILD_POCOXMLREADER_DLL THREAD
else
  COMMON_DEFS = THREAD PTHREADS
endif


check::
	@echo Target check...
	$(CHECK_SETTINGS)

prebuild::
	@echo Target $@
	$(MAKE_DIRECTORIES)

build:: $(REL_OBJ_DIR)$(XMLREADER_LIB)

$(REL_OBJ_DIR)$(XMLREADER_LIB): $(OBJ_FILES)
	@echo Target $@
	$(call COMPILE_WINRC,$(XMLREADER_LIB),$(REL_OBJ_DIR))
	cd $(REL_OBJ_DIR); \
	$(LINK_DLL) $(LD_DEST_FLAG)$(XMLREADER_LIB) $(OBJ_FILES) \
	            $(VER_RES_FILE) $(LIBPATH)
	$(call LIB_POST_BUILD,$@)
	$(call CREATE_SOFT_LINK,$(REL_OBJ_DIR),$(XMLREADER_LIB),\
               $(XMLREADER_LIB:$(VERSION_SUFFIX)=))

clean:
	rm -fr $(REL_OBJ_DIR)

install:
	$(call INSTALL_LIB,$(XMLREADER_LIB))
	$(call CREATE_SOFT_LINK,$(INSTALL_DIR)/lib,$(XMLREADER_LIB),\
               $(XMLREADER_LIB:$(VERSION_SUFFIX)=))

