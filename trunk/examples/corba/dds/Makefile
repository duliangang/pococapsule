all:
	@echo "Pls specify ORB"
	@echo "    vb: VisiBroker"
	@echo "   tao: TAO"

vb:
	$(MAKE) -f makefiles/Makefile.build$(OS) vb

tao:
	$(MAKE) -f makefiles/Makefile.build$(OS) tao

clean_vb: clean

clean_tao: clean

clean:
	$(MAKE) -f makefiles/Makefile.build$(OS) clean
