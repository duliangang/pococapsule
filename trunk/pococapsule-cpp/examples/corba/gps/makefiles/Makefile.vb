include ../stdmkdir/stdmk.vb

EXE = main TickGenImpl.so GPSLocatorImpl.so NavDisplayImpl.so reflx.so

all: Interfaces_c.cc setup_reflx.cc $(EXE)

Interfaces_c.cc: Interfaces.idl
	$(ORBCC) $<

main: main.C
	$(POCO_CC) $(CCFLAGS) -o $@ $< $(POCO_LIBPATH) $(LIBPOCO)

setup_reflx.cc: setup.xml
	$(POCO_PROJ) -s=_reflx.cc -h=GPSLocatorImpl.h -h=NavDisplayImpl.h -h=TickGenImpl.h $<

reflx.so: setup_reflx.cc
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ $< \
				$(POCO_LIBPATH) $(LIBPOCO) 

TickGenImpl.so: Interfaces_c.o TickGenImpl.C
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ \
	Interfaces_c.o TickGenImpl.C $(LIBPATH) $(LIBORB)

GPSLocatorImpl.so: Interfaces_c.o GPSLocatorImpl.C
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ \
	Interfaces_c.o GPSLocatorImpl.C $(LIBPATH) $(LIBORB)

NavDisplayImpl.so: Interfaces_c.o NavDisplayImpl.C
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ \
	Interfaces_c.o NavDisplayImpl.C $(LIBPATH) $(LIBORB)

clean:
	-rm -f core *.o *.so *.hh *.cc *_reflx.* main
