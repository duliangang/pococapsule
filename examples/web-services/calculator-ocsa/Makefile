include ${POCOCAPSULE_DIR}/examples/web-services/stdmk

all: calculator.cpp reflx.so client server

calculator.cpp: Calculator.wsdl
	${WSDL2CPP} Calculator.wsdl calculator

reflx.so: setup.xml calculatorImpl.cpp 
	${POCO_PROJ} -s=_reflx.cc -h=ComponentImpls.h -h=MyCalculatorImpl.h setup.xml 
	${CC} setup_reflx.cc calculatorStructs.cpp calculatorImpl.cpp \
		-L${POCOCAPSULE_DIR}/lib -l${WS_HELPER} -shared -o $@

client: client.C 
	${CC} client.C calculator.cpp calculatorStructs.cpp -L${WASPC_HOME}/lib -lwasp_stl -o client

server: server.C
	${CC} server.C -L${POCOCAPSULE_DIR}/lib -lpococapsule -o server

clean:
	rm -f client server calculator*.cpp calculator*.h *_reflx.* reflx.*
