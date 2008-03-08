#ifndef _MyNodeImplB_H_
# define _MyNodeImplB_H_

#include <stdio.h>
#include <string>

#include "Node.h"

class MyNodeImplB : public Node {
	std::string _name;
  public:
	MyNodeImplB(const char* name) : _name(name) {}

        void push(const char* msg, int count) {
                printf("B::push(\"%s\")\n", msg);

		if( count > 5 ) {
			printf("\n%s->end\n", msg);
			return;
		}

		if( msg == NULL ) {
			msg = "";
		}

		std::string s(msg);
		s += (const char*)"->B:";
		s += _name.c_str();
		next()->push(s.c_str(), ++count);
	}
};

#endif
