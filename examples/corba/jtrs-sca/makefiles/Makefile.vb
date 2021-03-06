include ../stdmkdir/stdmk.vb

EXE = main reflx.so scahelper.so sampleImpl.so

all: CF_c.cc CF_s.cc sample_c.cc sample_s.cc $(EXE)

CF_c.cc: CF.idl
	$(ORBCC) $<

CF_s.cc: CF.idl
	$(ORBCC) $<

sample_c.cc: sample.idl
	$(ORBCC) $<

sample_s.cc: sample.idl
	$(ORBCC) $<

main: main.C
	$(POCO_CC) $(CCFLAGS) -o $@ $< $(LIBPATH) $(LIBORB) \
				$(POCO_LIBPATH) $(LIBPOCO) 

reflx.so: setup.xml sad.xml srv.xml
	$(POCO_PROJ) -s=_reflx.cc -h=scahelper.h -h=sampleImpl.h setup.xml
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ setup_reflx.cc  \
			$(POCO_LIBPATH) $(LIBPOCO) $(LIB_POCO_CORBA_HELPER)

scahelper.so: scahelper.C CF_s.cc CF_c.cc
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ scahelper.C CF_s.cc CF_c.cc \
			$(LIBPATH) $(LIBORB)

sampleImpl.so: sampleImpl.C sample_c.cc sample_s.cc
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ \
	sample_c.cc sample_s.cc sampleImpl.C $(LIBPATH) $(LIBORB) 

clean:
	-rm -f core *.o *.so *.hh *.cc *_reflx.* main
