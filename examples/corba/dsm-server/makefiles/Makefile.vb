include ../stdmkdir/stdmk.vb

EXE = client server GreetingImpl.so reflx.so

all: Greeting_c.cc Greeting_s.cc setup_reflx.cc $(EXE)

Greeting_c.cc: Greeting.idl
	$(ORBCC) $<

Greeting_s.cc: Greeting.idl
	$(ORBCC) $<

client: Greeting_c.o client.C
	$(POCO_CC) $(CCFLAGS) -o $@ Greeting_c.o client.C $(LIBPATH) $(LIBORB)

server: server.C
	$(POCO_CC) $(CCFLAGS) -o $@ $< $(LIBPATH) $(LIBORB) \
				$(POCO_LIBPATH) $(LIBPOCO) 

setup_reflx.cc: setup.xml
	$(POCO_PROJ) -s=_reflx.cc -h=GreetingImpl.h $< 

reflx.so: setup_reflx.cc
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ $< \
				$(POCO_LIBPATH) $(LIBPOCO) $(LIB_POCO_CORBA_HELPER)

GreetingImpl.so: Greeting_c.o Greeting_s.o GreetingImpl.C
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ \
	Greeting_c.o Greeting_s.o GreetingImpl.C $(LIBPATH) $(LIBORB) 

clean:
	-rm -f core *.o *.so *.hh *.cc *_reflx.* client server
