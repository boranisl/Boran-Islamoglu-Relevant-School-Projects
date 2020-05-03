#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <string>
#include <vector>
#include <list>
using namespace std;

template <class HashedObj>
struct HashNode
{
	HashedObj word;
	HashedObj meaning;
	HashNode<HashedObj> * next;
	bool IsActive;
	HashNode<HashedObj> ()
	{
		next = nullptr;
		IsActive = false;
	}
	HashNode<HashedObj>(HashedObj word1, HashedObj meaning1)
	{
		word = word1;
		meaning = meaning1;
		next = nullptr;
	}
};

template <class HashedObj>
class HashTable
{
public:
	HashTable( const HashedObj & notFound, int size = 101 );

	const HashedObj & find( const HashedObj & x );

	void makeEmpty( );
	void insert( const HashedObj & x, const HashedObj & y);
	void remove( const HashedObj & x );
	void rehash();

	const HashTable & operator=( const HashTable & rhs);

private:
	vector<HashNode<HashedObj>> theLists;   
	// The array of Lists
	const HashedObj ITEM_NOT_FOUND;
	int currentsize;
};

#include "HashTable.cpp"

#endif