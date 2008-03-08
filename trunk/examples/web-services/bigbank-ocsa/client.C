#include <waspc/common.h>
#include <waspc/runtime/Runtime.h>

#include "accountSvc.h"
#include <iostream>

int main(int argc,char **argv)
{
	WASP_Runtime::clientInitialize();

	try {
		WASP_Runtime::clientStart("conf/client.xml", NULL);

		AccountServiceSOAP service;
		if( argc > 1 ) {
			//
			// if argv[1] is set, it should be in the form of:
			// "http://<server-host>:6070/AccountService"
			//
			service.stub.setAddress(argv[1], NULL);
		}

		std::string customerId("peter2006");

		::AccountReport report = service.getAccountReport(customerId);

		if( report == NULL ) {
			return 0;
		}

		unsigned i;
		char balanceStr[20];

		std::cout << "Account Report" << std::endl;
		std::cout << "==============" << std::endl;

		::StockAccountArray& stocks = report->stocks;
		
		for(i=0; i<stocks.size(); i++) {
			std::cout << "Stock Account  " << stocks[i]->accountNumber << std::endl;
			std::cout << "Stock Symbol : " << stocks[i]->symbol << std::endl;
			std::cout << "     Holding : " << stocks[i]->quantity << std::endl;
			sprintf(balanceStr, "%.2f", stocks[i]->balance);
			std::cout << "       Value : " << balanceStr << std::endl << std::endl;
		}

		::CheckingAccountArray& checking = report->checking;

		for(i=0; i<checking.size(); i++) {
			std::cout << "Checking Account " << checking[i]->accountNumber << std::endl;
			sprintf(balanceStr, "%.2f", checking[i]->balance);
			std::cout << "     Balance : " << balanceStr << std::endl << std::endl;
		}
	
		::SavingsAccountArray& savings = report->savings;

		for(i=0; i<savings.size(); i++) {
			std::cout << "Savings Account " << savings[i]->accountNumber << std::endl;
			sprintf(balanceStr, "%.2f", savings[i]->balance);
			std::cout << "     Balance : " << balanceStr << std::endl << std::endl;
		}

		std::cout << std::endl;
	}
	catch(WASP_Exception *exc) {
		std::cerr << "Exception: " << exc->getCharMessage() << std::endl;
		delete exc;
		return -1;
	}

	WASP_Runtime::clientTerminate();
	return 0;
}
