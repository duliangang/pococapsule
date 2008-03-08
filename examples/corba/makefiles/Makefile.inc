ifeq ($(POCOCAPSULE_DIR),)
$(error POCOCAPSULE_DIR not defined)
endif

include $(POCOCAPSULE_DIR)/examples/makefiles/Makefile.inc


ifeq ($(PLATFORM),VxWorks)
  define INSTALL_MK_FILE
    $(CP) $(POCOCAPSULE_DIR)/examples/corba/makefiles/Makefile.com $(TGT_DIR)/Makefile
    $(CP) makefiles/Makefile.taoVxWorks $(TGT_DIR)/Makefile.tao
    $(CP) makefiles/Makefile.vbVxWorks $(TGT_DIR)/Makefile.vb
  endef
  define INSTALL_REPL_MAIN
    $(CP) $1 $(TGT_DIR)/$1.tmp
    sed -e "s/main(/$(basename $1)_main(/g" -e 's/.xml/.ctx/g' $(TGT_DIR)/$1.tmp > $(TGT_DIR)/$1
    $(RM) $(TGT_DIR)/$1.tmp
  endef
else
  define INSTALL_REPL_MAIN
    $(CP) $1 $(TGT_DIR)/$1
  endef
  ifeq ($(PLATFORM),Windows_NT)
  define INSTALL_MK_FILE
    $(CP) $(POCOCAPSULE_DIR)/examples/corba/makefiles/Makefile.com $(TGT_DIR)/Makefile
    $(CP) makefiles/Makefile.taoWindows_NT $(TGT_DIR)/Makefile.tao.tmp
    $(CP) makefiles/Makefile.vbWindows_NT $(TGT_DIR)/Makefile.vb.tmp
    sed -e 's/stdmkdir\/stdmk.vb_win/stdmk.vb/g' $(TGT_DIR)/Makefile.vb.tmp >$(TGT_DIR)/Makefile.vb
    sed -e 's/stdmkdir\/stdmk.tao_win/stdmk.tao/g' $(TGT_DIR)/Makefile.tao.tmp >$(TGT_DIR)/Makefile.tao
    -rm $(TGT_DIR)/Makefile.vb.tmp $(TGT_DIR)/Makefile.tao.tmp 
  endef
  else
  endif
endif
