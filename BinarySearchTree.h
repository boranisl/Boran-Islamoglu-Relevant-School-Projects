#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H
#include <string>
using namespace std;

template <class Object>
struct TreeNode
{
	Object word;
	Object meaning;
	TreeNode<Object> * left;
	TreeNode<Object> * right;
	TreeNode<Object>(Object word1, Object meaning1)
	{
		word = word1;
		meaning = meaning1;
		left = nullptr;
		right = nullptr;
	}
};

template <class Object>
class BinarySearchTree
{
private:
	TreeNode<Object> * root;
public:
	BinarySearchTree<Object>();
	TreeNode<Object> GetRoot();
	void insert(Object word1, Object meaning1);
	void findAndUpdate(Object word1, Object meaning1);
	Object find(Object b);
	bool IsEmpty();
};

#include "BinarySearchTree.cpp"

#endif