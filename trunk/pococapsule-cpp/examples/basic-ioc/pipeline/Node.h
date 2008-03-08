#ifndef _Node_H_
# define _Node_H_

class Node {
	Node* _next;
  public:
	Node* next() { return _next; }
	void  next(Node* n) { _next = n; }

	virtual void push(const char* msg, int) = 0;
};

#endif
