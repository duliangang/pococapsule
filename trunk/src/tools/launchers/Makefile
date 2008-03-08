include ../../makeincs/Makefile.inc

PIC =
CCINCLUDE_DIRS = . $(JAVA_HOME)/include

ifeq ($(PLATFORM),Windows_NT)
  CCINCLUDE_DIRS += $(JAVA_HOME)/include/win32
  LINK_LIBS = 
else
  CCINCLUDE_DIRS += $(JAVA_HOME)/include/linux
  LINK_LIBS = -ldl
endif

CCINCLUDES = $(addprefix $(DELIMITER)I,$(CCINCLUDE_DIRS))

EXE = $(addprefix $(REL_OBJ_DIR), $(addsuffix $(BIN_SUFFIX), pxgenproxy pxencode pxtransform))

all: prebuild $(EXE) install

prebuild::
	@echo Target $@
	$(CHECK_SETTINGS)
	$(MAKE_DIRECTORIES)

$(REL_OBJ_DIR)pxgenproxy$(BIN_SUFFIX): pxgenproxy.$(OBJ_EXT) \
                        launcher.$(OBJ_EXT) pocostr.$(OBJ_EXT)
	cd $(REL_OBJ_DIR); \
	$(LINK) $(LD_DEST_FLAG)pxgenproxy$(BIN_SUFFIX) \
	pxgenproxy.$(OBJ_EXT) launcher.$(OBJ_EXT) pocostr.$(OBJ_EXT) \
	$(LIBPATH)
	$(call BIN_POST_BUILD,$@)

$(REL_OBJ_DIR)pxencode$(BIN_SUFFIX): pxencode.$(OBJ_EXT) \
                        launcher.$(OBJ_EXT) pocostr.$(OBJ_EXT)
	cd $(REL_OBJ_DIR); \
	$(LINK) $(LD_DEST_FLAG)pxencode$(BIN_SUFFIX) \
	pxencode.$(OBJ_EXT) launcher.$(OBJ_EXT) pocostr.$(OBJ_EXT) \
	$(LIBPATH)
	$(call BIN_POST_BUILD,$@)

$(REL_OBJ_DIR)pxtransform$(BIN_SUFFIX): pxtransform.$(OBJ_EXT) \
                        launcher.$(OBJ_EXT) pocostr.$(OBJ_EXT)
	cd $(REL_OBJ_DIR); \
	$(LINK) $(LD_DEST_FLAG)pxtransform$(BIN_SUFFIX) \
	pxtransform.$(OBJ_EXT) launcher.$(OBJ_EXT) pocostr.$(OBJ_EXT) \
	$(LIBPATH)
	$(call BIN_POST_BUILD,$@)

install:
	$(call INSTALL_BIN,pxgenproxy$(BIN_SUFFIX))
	$(call INSTALL_BIN,pxencode$(BIN_SUFFIX))
	$(call INSTALL_BIN,pxtransform$(BIN_SUFFIX))

clean:
	rm -f $(EXE)
