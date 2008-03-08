include ${POCOCAPSULE_DIR}/examples/web-services/stdmk

all: stockQuoteWS.cpp accountSvc.cpp Components.so client server

stockQuoteWS.cpp: StockQuoteWebService.wsdl
	${WSDL2CPP} StockQuoteWebService.wsdl stockQuoteWS

accountSvc.cpp: AccountService.wsdl
	${WSDL2CPP} AccountService.wsdl accountSvc

Components.so: rflxml Components.C
	${CC} setup_reflx.cc \
		accountSvcStructs.cpp accountSvcImpl.cpp \
		stockQuoteWS.cpp stockQuoteWSStructs.cpp \
		Components.C \
		-L${POCOCAPSULE_DIR}/lib -l${WS_HELPER} \
		-L${WASPC_HOME}/lib -lwasp_stl -shared -o $@

client: client.C
	${CC} client.C accountSvc.cpp accountSvcStructs.cpp -L${WASPC_HOME}/lib -lwasp_stl -o client

server: server.C
	${CC} server.C -L${POCOCAPSULE_DIR}/lib -lpococapsule -o server

rflxml: setup.xml
	${POCO_PROJ} -s=_reflx.cc -h=Components.h setup.xml 

clean:
	rm -f client server stockQuoteWS* accountSvc* Components.so *_reflx.*
