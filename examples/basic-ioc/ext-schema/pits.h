/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _pits_h_
# define _pits_h_

class Pit {
  protected:
	char	_name[64];

  public:
	Pit(const char* name) { strcpy(_name, name); };

	virtual ~Pit() {}
	virtual void voice() = 0;
};

class Cat : public Pit {
  public:
	Cat(const char* name) : Pit(name) { };

	void voice() { printf("Cat %s meow....meow\n", _name); }
};

class Dog : public Pit {
  public:
	Dog(const char* name) : Pit(name) { }

	void voice() { printf("Dog %s bow...wow\n", _name); }
};

class Bird : public Pit {

  public:
	Bird(const char* name) : Pit(name) { }

	void voice() { printf("Bird %s ... singing ...\n", _name); }
};

#include <map>
#include <string>
#include <iostream>

void see_pits(std::map<std::string, Pit*> &pits)
{
	std::map<std::string, Pit*>::iterator cur = pits.begin();

	std::cout << "------------------ all pits ---------------------" << std::endl; 
	for(;cur!= pits.end();cur++) {
		cur->second->voice();
	}
}

#endif
