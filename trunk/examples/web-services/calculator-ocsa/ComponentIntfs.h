#ifndef _component_interfaces_h_
# define _component_interfaces_h_

class AddService {
  public:
	virtual ~AddService() {}
	virtual double add(double n1, double n2) = 0;
};

class SubtractService {
  public:
	virtual ~SubtractService() {}
	virtual double subtract(double n1, double n2) = 0;
};

class MultiplyService {
  public:
	virtual ~MultiplyService() {}
	virtual double multiply(double n1, double n2) = 0;
};

class DivideService {
  public:
	virtual ~DivideService() {}
	virtual double divide(double n1, double n2) = 0;
};

#endif
