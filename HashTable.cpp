#include <iostream>
#include "HashTable.h"
#include <string>
using namespace std;

int hash (const int key, const int tableSize)
{ 
	return  (key % tableSize);
}

int hash (const string & key, int tableSize)
{ 
	int hashVal = 0;

	for (int i = 0; i < key.length();i++) 
		hashVal = 37 * hashVal + key[ i ];

	hashVal = hashVal % tableSize;

	if (hashVal < 0) 
		hashVal = hashVal + tableSize;

	return(hashVal);
}


bool isPrime( int n )
{
	if ( n == 2 || n == 3 )
		return true;

	if ( n == 1 || n % 2 == 0 )
		return false;

	for ( int i = 3; i * i <= n; i += 2 )
		if ( n % i == 0 )
			return false;

	return true;
}


int nextPrime( int n )
{
	if ( n % 2 == 0 )
		n++;

	for ( ; ! isPrime( n ); n += 2 )
		;

	return n;
}

template <class HashedObj>
HashTable<HashedObj>::HashTable( 
	const HashedObj & notFound, int size )
	: ITEM_NOT_FOUND( notFound ), theLists( nextPrime( size ) )
{
	currentsize = 0;
}

template <class HashedObj>
void HashTable<HashedObj>::makeEmpty( )
{
	for( int i = 0; i < theLists.size( ); i++ )
		theLists[ i ].makeEmpty( );  
	// destroy the lists but not the vector!
}

template <class HashedObj>
void HashTable<HashedObj>::insert( const HashedObj & x, const HashedObj & y)
{
	HashNode<HashedObj> * ptr = &theLists[ ::hash( x, theLists.size( ) ) ];
	bool isUnique=true;
	while (ptr->next)
	{
		if (ptr->word == x && ptr->meaning == y)
		{
			isUnique = false;
			break;
		}
		ptr = ptr->next;
	}
	if (isUnique == true)
	{
		ptr->next = new HashNode<HashedObj>(x,y);
		ptr->next->IsActive = true;
		currentsize++;
	}
	if (++currentsize > theLists.size()/2)
	{
		rehash();
	}
}

template <class HashedObj>
void HashTable<HashedObj>::remove( const HashedObj & x )
{

}

template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find( const HashedObj & x )
{
	HashNode<HashedObj> * ptr = &theLists[ ::hash( x, theLists.size( ) ) ];
	if (ptr->next != nullptr)
	{
		while (ptr->next)
		{
			if (ptr->next->word == x)
			{
				return ptr->next->meaning;
			}
			ptr= ptr->next;
		}
	}
	return ITEM_NOT_FOUND;
}

template <class HashedObj>

void HashTable<HashedObj>::rehash( )

{
	vector<HashNode<HashedObj>> old = theLists;
	theLists.clear();
	currentsize = 0;
	theLists.resize(nextPrime(old.size()*2));

	for (int i = 0; i < old.size(); i++)
	{
		HashNode<HashedObj> * ptr = &old[i];
		if (ptr->next != nullptr)
		{
			if (ptr->next->IsActive == true)
			{
				while (ptr->next)
				{
					insert(ptr->next->word,ptr->next->meaning);
					ptr= ptr->next;
				}
			}
		}
	}
	float l = theLists.size();
	float o = old.size();
	float loadFactor = ((o/2)+2)/l;
	cout << "rehashed..." << endl;
	cout << "previous table size:" << old.size() << ", new table size:" << theLists.size() << ", current unique word count:" << (old.size()/2)+1 << "," << endl;
	cout <<	"current load factor:" << loadFactor << endl;
}




