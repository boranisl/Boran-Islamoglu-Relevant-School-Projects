#ifndef STACK_H
#define STACK_H

#include <string>
using namespace std;

template <class Object>
struct Node
{
	Object value;
	Node<Object> *next;
};

template<class Object>
class Stack
{
	public:
		Node<Object> * top;
		Stack<Object>();
		void push(Object object);
		void pop();
		bool isEmpty();
};

#endif

#include "Stack.cpp"