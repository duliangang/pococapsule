ifeq ($(POCOCAPSULE_DIR),)
$(error POCOCAPSULE_DIR not defined)
endif

include $(POCOCAPSULE_DIR)/examples/makefiles/Makefile.inc

ifeq ($(PLATFORM),VxWorks)
  define INSTALL_MK_FILE
    $(CP) makefiles/Makefile.VxWorks $(TGT_DIR)/Makefile
  endef
  define INSTALL_MAIN
    $(SED) -e 's/int main(/int poco_main(/g' -e 's/setup.xml/setup.ctx/g' \
      main.C >$(TGT_DIR)/main.C
  endef
else
  define INSTALL_MAIN
	@$(CP) main.C $(TGT_DIR)/main.C
  endef
  define INSTALL_MK_FILE
	@if [ -f makefiles/Makefile.build$(PLATFORM) ]; then \
	  $(SED) -e 's/stdmkdir\///g' -e 's/stdmk.win/stdmk/g' makefiles/Makefile.build$(PLATFORM) \
	    > $(TGT_DIR)/Makefile;\
	else \
	  $(SED) -e 's/stdmkdir\///g' makefiles/Makefile.build \
	    >$(TGT_DIR)/Makefile; \
	fi;
  endef
endif


