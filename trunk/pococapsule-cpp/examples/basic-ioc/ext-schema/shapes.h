/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _shapes_h_
# define _shapes_h_

#include <stdio.h>

class Shape {
  protected:
	char	_name[64];

  public:
	Shape(const char* name) { strcpy(_name, name); };

	virtual ~Shape() {}
	virtual void print() = 0;
};

class Square : public Shape {
	int	_a;

  public:
	Square(const char* name) : Shape(name) { _a = 0; };
	void a(int x) { _a = x; }

	void print() { printf("Square %s has side length: %d\n", _name, _a); }
};

class Triangle : public Shape {
	int	_a;
	int	_b;
	int	_c;

  public:
	Triangle(const char* name) : Shape(name) { _a = 0; _b = 0; _c = 0; }
	void a(int x) { _a = x; }
	void b(int x) { _b = x; }
	void c(int x) { _c = x; }

	void print() { printf("Triangle %s has side lengths: %d %d %d\n", _name, _a, _b, _c); }
};

class Circle : public Shape {
	int	_r;

  public:
	Circle(const char* name) : Shape(name) { _r = 0; }
	void r(int x) { _r = x; }

	void print() { printf("Circle %s has radius length: %d\n", _name, _r); }
};

#include <map>
#include <string>
#include <iostream>

void print_shapes(std::map<std::string, Shape*> &shapes)
{
	std::map<std::string, Shape*>::iterator cur = shapes.begin();

	std::cout << "------------------ all shapes -------------------" << std::endl; 
	for(;cur!= shapes.end();cur++) {
		cur->second->print();
	}
}

#endif
