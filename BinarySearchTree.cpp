#include <iostream>
#include "BinarySearchTree.h"
#include <string>
#include <algorithm>
using namespace std;

template <class Object>
BinarySearchTree<Object>::BinarySearchTree()
{
	root=nullptr;
}
template <class Object>
TreeNode<Object> BinarySearchTree<Object>::GetRoot()
{
	return root;
}
template <class Object>
void BinarySearchTree<Object>::insert(Object b,Object meaning1)
{
	TreeNode<Object> * ptr; bool IsANewWord = true; string word1 = b;
	if (root == nullptr)
	{
		root = new TreeNode<Object>(word1,meaning1);
		ptr = root;
	}
	else
	{
		ptr = root; bool IsLeft; int i = 0;
		while (i < word1.length())
		{
			if ('A' <= word1.at(i) <= 'Z')
			{
				word1.at(i) = tolower(word1.at(i));
			}
			i++;
		}
		while (ptr)
		{
			if (word1 < ptr->word)
			{
				if (ptr->left == nullptr)
				{
					IsLeft = true;
					break;
				}
				else
				{
					ptr = ptr->left;
				}
			}
			else if (ptr->word < word1)
			{
				if (ptr->right == nullptr)
				{
					IsLeft = false;
					break;
				}
				else
				{
					ptr = ptr->right;
				}
			}
			else
			{
				cout << "Word already exists, if you want to add new translation please use command 2." << endl;
				IsANewWord = false; break;
			}
		}
		if (IsANewWord == true)
		{
			if (IsLeft == true)
			{
				ptr->left = new TreeNode<Object>(b,meaning1);
			}
			else
			{
				ptr->right = new TreeNode<Object>(b,meaning1);
			}
		}

	}
}

template <class Object>
Object BinarySearchTree<Object>::find(Object b)
{
	TreeNode<Object> * ptr = root;
	while (ptr)
	{
		string word1 = b; int i=0;
		while (i < word1.length())
		{
			if ('A' <= word1.at(i) <= 'Z')
			{
				word1.at(i) = tolower(word1.at(i));
			}
			i++;
		}
		if (word1 < ptr->word && b != ptr->word)
		{
			ptr = ptr->left;

		}
		else if (ptr->word < word1 && b != ptr->word)
		{
			ptr = ptr->right;

		}
		else
		{
			return ptr->meaning;
		}
	}
	return "-1";
}

template <class Object>
void BinarySearchTree<Object>::findAndUpdate(Object b, Object meaning1)
{
	TreeNode<Object> * ptr = root; string word1=b;
	int i = 0;
	while (i < word1.length())
	{
		if ('A' <= word1.at(i) <= 'Z')
		{
			word1.at(i) = tolower(word1.at(i));
		}
		i++;
	}
	while (ptr)
	{
		if (word1 < ptr->word && b != ptr->word)
		{
			ptr = ptr->left;
		}
		else if (ptr->word < word1 && b != ptr->word)
		{
			ptr = ptr->right;
		}
		else
		{
			string a = ptr->meaning + ", " + meaning1;
			ptr->meaning = a;
			break;
		}
	}
}

template <class Object>
bool BinarySearchTree<Object>::IsEmpty()
{
	if (root == nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}
