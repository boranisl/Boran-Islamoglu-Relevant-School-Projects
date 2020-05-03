#include <iostream>
using namespace std;

template <class Object>
Stack<Object>::Stack()
{
	top=nullptr;
}
template <class Object>
void Stack<Object>::push(Object object)
{
	Node<Object> *newNode;

	newNode= new Node<Object>;
	newNode->value = object;

	if(isEmpty())
	{
		top = newNode;
		newNode->next = nullptr;
	}
	else
	{
		newNode->next = top;
		top = newNode;
	}
}
template <class Object>
void Stack<Object>::pop()
{
	Node<Object> *temp;

	if(isEmpty())
	{
	}

	else 
	{
		temp = top->next;
		delete top;
		top = temp;
	}
}
template <class Object>
bool Stack<Object>::isEmpty()
{
	bool empty;

	if(top==nullptr)
		empty=true;

	else 
		empty=false;

	return empty;
}