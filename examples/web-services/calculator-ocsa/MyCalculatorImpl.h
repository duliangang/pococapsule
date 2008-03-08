#ifndef _calculator_impl_h
# define _calculator_impl_h

#include "ComponentIntfs.h"

#include "calculatorImpl.h" // skel generated from WASPC
typedef CalculatorPortImpl CalculatorServiceSkel; // the CalculatorServiceImpl is WASPC generated skeleton

class MyCalculatorServiceImpl : public CalculatorServiceSkel
{

	AddService* _addService;
	SubtractService* _subtractService;
	MultiplyService* _multiplyService;
	DivideService* _divideService;

  public:
	//
	// setters to inject component references
	//
	void addService(AddService* service) {
		_addService = service;
	}

	void subtractService(SubtractService* service) {
		_subtractService = service;
	}

	void divideService(DivideService* service) {
		_divideService = service;
	}

	void multiplyService(MultiplyService* service) {
		_multiplyService = service;
	}

	//
	// service methods to invoke components
	//
	float add(float n1, float n2) {
		return _addService->add(n1, n2);
	}

	float sub(float n1, float n2) {
		return _subtractService->subtract(n1, n2);
	}

	float mul(float n1, float n2) {
		return _multiplyService->multiply(n1, n2);
	}

	float div_1(float n1, float n2) {
		return _divideService->divide(n1, n2);
	}
};

#endif
