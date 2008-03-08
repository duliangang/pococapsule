#include "Components.h"

#include <string>
#include <iostream>

static float _currencyConvert(const std::string& currency, float value)
{
	const char* cur = currency.c_str();

	if( cur == NULL) {
		return 0.0f;
	}

	std::cout << "Currency is : " << cur << std::endl;
	if( !strcmp(cur, "USD") ) {
		return value;
	}
	else 
	if( !strcmp(cur, "EURO") ) {
		return 0.8f*value;
	}

	return 0.0f;
}

::AccountReport AccountServiceImpl::getAccountReport(const std::string& customerID)
{
	::AccountReport report;

	if( _dataService == NULL || _stockService == NULL ) {
		return report;
	}

	::CheckingAccount checking = _dataService->getCheckingAccount(customerID);
	checking->balance = _currencyConvert(_currency, checking->balance);
	report->checking.insert(report->checking.end(), checking);

	::SavingsAccount savings = _dataService->getSavingsAccount(customerID);
	savings->balance = _currencyConvert(_currency, savings->balance);
	report->savings.insert(report->savings.end(), savings);

	::StockAccount stocks = _dataService->getStockAccount(customerID);
	float value = _stockService->getQuote(stocks->symbol);
	stocks->balance = _currencyConvert(_currency, value* atoi(stocks->quantity.c_str()));
	report->stocks.insert(report->stocks.end(), stocks);

	return report;	
}

float StockQuoteService::getQuote(const std::string& symbol)
{
	std::cout << "GetQuote on www.webserviceX.net, pls wait ..." << std::endl;

	// issue a SOAP query. The same service is also 
	// available via REST on URI: 
	// http://www.webservicex.net/stockquote.asmx/GetQuote?symbol=<the-symbol>
	std::string result = _stockWS->GetQuote(symbol);

	const char* h = strstr(result.c_str(), "<Last>") + strlen("<Last>");
	double price = atof(h);
	char priceStr[20];
	sprintf(priceStr, "%.2lf", price);
	std::cout << "Stock " << symbol << " last price is : "
	          << priceStr << std::endl;
		
	return price;
}

::CheckingAccount AccountDataService::getCheckingAccount(const std::string& id)
{
	::CheckingAccount account;

	account->accountNumber = id + "_CHA12345";
	account->balance = 1500.0f;

	return account;
	
}

::SavingsAccount  AccountDataService::getSavingsAccount(const std::string& id)
{
	::SavingsAccount account;

	account->accountNumber = id + "_SAA12345";
	account->balance = 2500.0f;

	return account;
}

::StockAccount    AccountDataService::getStockAccount(const std::string& id)
{
	::StockAccount account;

	account->accountNumber = id + "_STA12345";
	account->symbol = "IBM";
	account->quantity = "1000";

	return account;
}
