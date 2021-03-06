include ${POCOCAPSULE_DIR}/examples/corba/stdmkdir/stdmk.vb

EXE = libpocodds.so 

all: dds_dcps_c.hh typesupport_c.hh $(EXE)

dds_dcps_c.cc, dds_dcps_c.hh: dds_dcps.idl
	$(ORBCC) $<

typesupport_c.hh: typesupport.idl
	$(ORBCC) $<

libpocodds.so: factory.C participant.C publisher.C subscriber.C dds_dcps_c.cc
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ \
	factory.C  participant.C  publisher.C  subscriber.C dds_dcps_c.cc \
	$(LIBPATH) $(LIBORBNTFY) $(POCO_LIBPATH) $(LIB_POCO_CORBA_HELPER)

clean:
	-rm -f core *.o *.so *.hh *.cc 
