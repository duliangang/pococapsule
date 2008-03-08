include ../stdmkdir/stdmk.vb

EXE = publisher subscriber EmitterImpl.so ListenerImpl.so \
	publish_reflx.so subscribe_reflx.so

idlstubs = MyTypedEvent_c.cc MyTypedEvent_s.cc

all: publish_reflx.cc subscribe_reflx.cc $(EXE)

MyTypedEvent_c.cc: MyTypedEvent.idl
	$(ORBCC) -corba_style_tie $<

MyTypedEvent_s.cc: MyTypedEvent.idl
	$(ORBCC) -corba_style_tie $<

publisher: publisher.C
	$(POCO_CC) $(CCFLAGS) -o $@ $< $(LIBPATH) $(LIBORB) \
		$(POCO_LIBPATH) $(LIBPOCO) 

subscriber: subscriber.C 
	$(POCO_CC) $(CCFLAGS) -o $@ $< $(LIBPATH) $(LIBORB) \
		$(POCO_LIBPATH) $(LIBPOCO) 

publish_reflx.cc: publish.xml
	$(POCO_PROJ) -s=_reflx.cc -h=EmitterImpl.h $<

publish_reflx.so: publish_reflx.cc
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ $< \
		$(POCO_LIBPATH) $(LIBPOCO) $(LIB_POCO_CORBA_HELPER)

subscribe_reflx.cc: subscribe.xml
	$(POCO_PROJ) -s=_reflx.cc -h=ListenerImpl.h $<

subscribe_reflx.so: subscribe_reflx.cc
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ $< \
		$(POCO_LIBPATH) $(LIBPOCO) $(LIB_POCO_CORBA_HELPER)

EmitterImpl.so: EmitterImpl.C MyTypedEvent_c.o
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@  \
	EmitterImpl.C MyTypedEvent_c.o \
	$(LIBPATH) $(LIBORBNTFY) 

ListenerImpl.so: ListenerImpl.C MyTypedEvent_c.o MyTypedEvent_s.o
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ \
	ListenerImpl.C MyTypedEvent_c.o MyTypedEvent_s.o \
	$(LIBPATH) $(LIBORBNTFY)

clean:
	-rm -f core *.o *.so *.hh *.cc *_reflx.* subscriber publisher 
