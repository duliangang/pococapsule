all:
	@echo "Pls specify ORB"
	@echo "    vb: VisiBroker"
	@echo "   tao: TAO"

vb:
	$(MAKE) -f makefiles/Makefile.build$(OS) vb

tao:
	$(MAKE) -f makefiles/Makefile.build$(OS) tao

clean_vb:
	$(MAKE) -f makefiles/Makefile.build$(OS) clean_vb

clean_tao:
	$(MAKE) -f makefiles/Makefile.build$(OS) clean_tao

clean: clean_vb clean_tao
