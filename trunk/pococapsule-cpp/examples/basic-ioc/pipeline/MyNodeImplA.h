#ifndef _MyNodeImplA_H_
# define _MyNodeImplA_H_

#include <stdio.h>
#include <string>

#include "Node.h"

class MyNodeImplA : public Node {
	std::string _name;
  public:
	MyNodeImplA(const char* name) : _name(name) {}

	void push(const char* msg, int count) {
		printf("A::push(\"%s\")\n", msg);

		if( count > 5 ) {
			printf("\n%s->end\n", msg);
			return;
		}

		if( msg == NULL ) {
			msg = "";
		}

		std::string s(msg);
		s += (const char*)"->A:";
		s += _name.c_str();
		next()->push(s.c_str(), ++count);
	}
};

#endif
