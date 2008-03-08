include ${POCOCAPSULE_DIR}/examples/web-services/stdmk

all: hello.cpp bizlogic.so client server

hello.cpp: Hello.wsdl
	${WSDL2CPP} Hello.wsdl hello

bizlogic.so: setup.xml 
	${POCO_PROJ} -s=_reflx.cc -h=MyHelloServiceImpl.h setup.xml 
	${CC} setup_reflx.cc helloStructs.cpp helloImpl.cpp \
		-L${POCOCAPSULE_DIR}/lib -l${WS_HELPER} -shared -o $@

client: client.C 
	${CC} client.C hello.cpp helloStructs.cpp -L${WASPC_HOME}/lib -lwasp_stl -o client

server: server.C
	${CC} server.C -L${POCOCAPSULE_DIR}/lib -lpococapsule -o server

clean:
	rm -f client server hello* bizlogic.so service_* *_reflx.*
