include ./makeincs/Makefile.inc

all:
	cd capsule; $(MAKE)
	cd tools/com/pocomatic/tools; $(MAKE) $(BUILD_TYPE)
  ifneq ($(PLATFORM),VxWorks)
	cd xmlreader; $(MAKE)
	cd xslreader; $(MAKE)
	cd tools/com/pocomatic/tools; $(MAKE) $(BUILD_TYPE)
	cd tools/launchers; $(MAKE)
	cd corbahelper; $(MAKE)
	cd wshelper; ${MAKE}
  endif

clean:
	cd capsule; $(MAKE) clean
	cd tools/com/pocomatic/tools; $(MAKE) clean
  ifneq ($(PLATFORM),VxWorks)
	cd xmlreader; $(MAKE) clean
	cd xslreader; $(MAKE) clean
	cd tools/launchers; $(MAKE) clean
	cd corbahelper; $(MAKE) clean
	cd wshelper; ${MAKE} clean
  endif
