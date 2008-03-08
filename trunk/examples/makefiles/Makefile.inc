
ifeq ($(POCOCAPSULE_DIR),)
$(error POCOCAPSULE_DIR not defined)
endif

INSTALL_DIR = $(POCOCAPSULE_DIR)/install/examples

CP = cp -f
MKDIR = mkdir -p
SED = sed

TGT_DIR = $(INSTALL_DIR)/$(REL_DIR)

