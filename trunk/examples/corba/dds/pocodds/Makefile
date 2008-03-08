all:
	@echo "Pls specify ORB"
	@echo "    vb: VisiBroker"
	@echo "   tao: TAO"

vb:
	$(MAKE) -f makefiles/Makefile.vb$(OS) all

tao:
	$(MAKE) -f makefiles/Makefile.tao$(OS) all

clean_vb:
	$(MAKE) -f makefiles/Makefile.vb$(OS) clean

clean_tao:
	$(MAKE) -f makefiles/Makefile.tao$(OS) clean

clean: clean_vb clean_tao
