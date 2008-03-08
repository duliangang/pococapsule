include ../stdmkdir/stdmk.vb

EXE = writer reader EmitterImpl.so ListenerImpl.so \
	writer_reflx.so reader_reflx.so

all: writer_reflx.cc reader_reflx.cc $(EXE)

MyDataTypeDefs_c.cc: MyDataTypeDefs.idl
	$(ORBCC) -I./pocodds -type_code_info -DUSE_POCO_DDS $<

MyDataTypeDefs_c.o: MyDataTypeDefs_c.cc
	$(POCO_CC) -c -I./pocodds $(CCFLAGS) -o $@ $< 

writer: writer.C
	$(POCO_CC) $(CCFLAGS) -o $@ $< $(LIBPATH) $(LIBORB) \
		$(POCO_LIBPATH) $(LIBPOCO) 

reader: reader.C 
	$(POCO_CC) $(CCFLAGS) -o $@ $< $(LIBPATH) $(LIBORB) \
		$(POCO_LIBPATH) $(LIBPOCO) 

writer_reflx.cc: writer.xml
	$(POCO_PROJ) -s=_reflx.cc -h=MyDataTypeImpls.h -h=EmitterImpl.h $<

writer_reflx.so: writer_reflx.cc
	$(POCO_CC) -I./pocodds -DUSE_POCO_DDS $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ $< \
		$(POCO_LIBPATH) $(LIBPOCO) $(LIB_POCO_CORBA_HELPER)

reader_reflx.cc: reader.xml
	$(POCO_PROJ) -s=_reflx.cc -h=MyDataTypeImpls.h -h=ListenerImpl.h $<

reader_reflx.so: reader_reflx.cc
	$(POCO_CC) -I./pocodds -DUSE_POCO_DDS $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ $< \
		$(POCO_LIBPATH) $(LIBPOCO) $(LIB_POCO_CORBA_HELPER)

EmitterImpl.so: EmitterImpl.C MyDataTypeDefs_c.o
	$(POCO_CC) -I./pocodds -DUSE_POCO_DDS $(CCFLAGS) $(DLL_LD_FLAGS) -o $@  \
	EmitterImpl.C MyDataTypeDefs_c.o \
	$(LIBPATH) $(LIBORBNTFY) 

ListenerImpl.so: ListenerImpl.C MyDataTypeDefs_c.o 
	$(POCO_CC) -I./pocodds -DUSE_POCO_DDS $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ \
	ListenerImpl.C MyDataTypeDefs_c.o \
	$(LIBPATH) $(LIBORBNTFY)

clean:
	-rm -f core *.o *.so *.hh *.cc *_reflx.* reader writer 
