#ifndef _my_components_h
#define _my_components_h

#include <string>
#include "accountSvcImpl.h"
#include "stockQuoteWS.h"

class AccountDataService;
class StockQuoteService;

typedef AccountServiceSOAPImpl AccountServiceSOAPSkel; // AccountServiceSOAPImpl is WASPC generated skeleton.

class AccountServiceImpl : public AccountServiceSOAPSkel
{
	std::string	_currency;
	AccountDataService*	_dataService;
	StockQuoteService*	_stockService;

  public:
	AccountServiceImpl() { _dataService = NULL; _stockService = NULL; }

	// setters
	void accountDataService(AccountDataService* service) { _dataService = service; }
	void stockQuoteService(StockQuoteService* service) { _stockService = service; }
	void currency(const std::string& curr) { _currency = curr; }
 
	// service operations
	::AccountReport getAccountReport (const std::string& customerID);
};

class StockQuoteService 
{
	StockQuoteSoap*	_stockWS;

  public:
	StockQuoteService() { _stockWS = NULL; }

	// setters
	void webService(StockQuoteSoap* service) { _stockWS = service; }

	// service operations
	float getQuote(const std::string& symbol);
};

class AccountDataService 
{
  public:
	// service operations
	::CheckingAccount getCheckingAccount(const std::string& id);
	::SavingsAccount  getSavingsAccount(const std::string& id);
	::StockAccount    getStockAccount(const std::string& id);
};
	
#endif

