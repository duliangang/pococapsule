include $(VBROKERDIR)/examples/vbroker/stdmk

POCO_PROJ=$(POCOCAPSULE_DIR)/bin/pxgenproxy -r=gather

CCINCLUDES = -I. -I$(VBROKERDIR)/include -I$(VBROKERDIR)/include/visinotify
POCO_LIBPATH=-L$(POCOCAPSULE_DIR)/lib
LIBPOCO=-lpococapsule
LIB_POCO_CORBA_HELPER=-lpoco2vb

POCO_INC=-I$(POCOCAPSULE_DIR)/include 
POCO_CC=$(CC) -DUSE_VBROKER $(CCFLAGS) $(POCO_INC) 

