#ifndef _components_impl_h
# define _components_impl_h

#include "ComponentIntfs.h"

class AddServiceImpl : public AddService {
  public: 
	double add(double n1, double n2) {
		return n1 + n2;
	}
};

class SubtractServiceImpl : public SubtractService {
  public:
	 double subtract(double n1, double n2) {
		return n1 - n2;
	}
};

class MultiplyServiceImpl : public MultiplyService {
  public:
	double multiply(double n1, double n2) {
		return n1 * n2;
	}
};

class DivideServiceImpl : public DivideService {
  public:
	 double divide(double n1, double n2) {
		return n1 / n2;
	}

};

#endif
