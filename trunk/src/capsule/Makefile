.PHONY : check prebuild

CXX_EXT = C

all: check prebuild build install

SRC_FILES = \
  appctxtimp.C \
  domreader.C \
  pocoenv.C \
  table.C \
  convert.C \
  hashtable.C \
  pocoapp.C \
  pocoreflx.C \
  util.C \
  descs.C \
  libloader.C \
  pocodom.C \
  pocostr.C

include ../makeincs/Makefile.inc

EXPORT_HEADERS = 

ifeq ($(PLATFORM), Windows_NT)
  LINK_LIBS =
  COMMON_DEFS = BUILD_POCOCAPSULE_DLL
else
  ifeq ($(PLATFORM),VxWorks)
    LINK_LIBS = 
  else
    LINK_LIBS = -ldl
  endif
endif

CAPSULE_LIB = $(LIB_PREFIX)pococapsule$(LIB_SUFFIX)$(VERSION_SUFFIX)
# eg. libpococapsule.so.1.0.0   on UNIX

CCINCLUDES = $(DELIMITER)I. $(DELIMITER)I../../include

check::
	@echo Target check...
	$(CHECK_SETTINGS)

prebuild::
	@echo Target $@
	$(MAKE_DIRECTORIES)

build:: $(REL_OBJ_DIR)$(CAPSULE_LIB)

$(REL_OBJ_DIR)$(CAPSULE_LIB): $(OBJ_FILES)
	$(call CREATE_LIBRARY,$(CAPSULE_LIB))

clean:
	rm -fr $(REL_OBJ_DIR)

install:
	$(call INSTALL_LIB,$(CAPSULE_LIB))

