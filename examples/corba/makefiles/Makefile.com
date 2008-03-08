all:
	@echo "Pls specify ORB"
	@echo "    vb: VisiBroker"
	@echo "   tao: TAO"

vb: 
	$(MAKE) -f Makefile.vb all

tao:
	$(MAKE) -f Makefile.tao all

clean_vb:
	$(MAKE) -f Makefile.vb clean

clean_tao:
	$(MAKE) -f Makefile.tao clean

clean: clean_vb clean_tao
