include ../stdmkdir/stdmk.vb

EXE = stubs.so main reflx.so \
		RangeScannerComponentImpl.so ServoComponentImpl.so \
		VehicleControllerImpl.so BumperSensorComponentImpl.so

all: rtc_lite_c.cc sample_c.cc $(EXE)

rtc_lite_c.cc: rtc_lite.idl
	$(ORBCC) $<

sample_c.cc: sample.idl
	$(ORBCC) $<

main: main.C
	$(POCO_CC) $(CCFLAGS) -o $@ $< $(LIBPATH) $(LIBORB) \
				$(POCO_LIBPATH) $(LIBPOCO) 

reflx.so: setup.xml
	$(POCO_PROJ) -s=_reflx.cc -h=VehicleControllerImpl.h \
			-h=RangeScannerComponentImpl.h -h=ServoComponentImpl.h \
			-h=BumperSensorComponentImpl.h setup.xml
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ setup_reflx.cc  $(POCO_LIBPATH) $(LIBPOCO) 

stubs.so: rtc_lite_c.cc sample_c.cc 
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ \
	rtc_lite_c.cc sample_c.cc $(POCO_LIBPATH) $(LIBPOCO)

RangeScannerComponentImpl.so: RangeScannerComponentImpl.C 
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ \
			RangeScannerComponentImpl.C \
			$(LIBPATH) $(LIBORB) 

ServoComponentImpl.so: ServoComponentImpl.C
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ ServoComponentImpl.C \
			$(LIBPATH) $(LIBORB) 

BumperSensorComponentImpl.so: BumperSensorComponentImpl.C
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ \
			BumperSensorComponentImpl.C \
			$(LIBPATH) $(LIBORB) 

VehicleControllerImpl.so: VehicleControllerImpl.C
	$(POCO_CC) $(CCFLAGS) $(DLL_LD_FLAGS) -o $@ \
			VehicleControllerImpl.C \
			$(LIBPATH) $(LIBORB) 

clean:
	-rm -f core *.o *.so *.hh *.cc *_reflx.* main
