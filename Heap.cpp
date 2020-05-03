#include <iostream>
#include "Heap.h"
#include <string>
using namespace std;
template <class Comparable>

BinaryHeap<Comparable>::BinaryHeap(int capacity)
{
	array1.resize(capacity);
	currentSize = 0;
}

template <class Comparable>

bool BinaryHeap<Comparable>::isEmpty() const
{
	if (currentSize == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <class Comparable>

bool BinaryHeap<Comparable>::isFull() const
{
	if (currentSize == array1.size() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <class Comparable>

void BinaryHeap<Comparable>::insert(Comparable & x )

{

	if ( isFull( ) )
	{
	}

	else
	{
			int hole = ++currentSize;

		for ( ; hole > 1 && x < array1[ hole / 2 ]; hole /= 2 )

			array1[ hole ] = array1[ hole / 2 ];

		array1[ hole ] = x;
	}
}

template <class Comparable>

Comparable BinaryHeap<Comparable>::getMin( )

{
	return array1[1];
}

template <class Comparable>

void BinaryHeap<Comparable>::deleteMin( )

{

	if ( isEmpty( ) )
	{
	}
	else
	{

		array1[ 1 ] = array1[ currentSize ];

		currentSize--;

		percolateDown( 1 );
	}


}

template <class Comparable>

void BinaryHeap<Comparable>::buildHeap(vector<Comparable> Input)

{

	array1 = Input; // copy input array1 to private array1;

	currentSize = array1.size() - 1;

	for ( int i = currentSize / 2; i > 0; --i )

		percolateDown( i );

}

template <class Comparable>

void BinaryHeap<Comparable>::percolateDown( int hole )

{

	int child;

	Comparable tmp = array1[ hole ]; // tmp is the item that will

	// be percolated down

	for( ; hole * 2 <= currentSize; hole = child )

	{

		if (hole * 2 <= currentSize)
		{
			child = hole * 2;
		}
		else
		{
			break;
		}

		if ( child != currentSize && array1[ child + 1 ] < array1[ child ] )

			child++;


		if (child <= currentSize && array1[ child ] < tmp )

			array1[ hole ] = array1[ child ];

		else

			break;
	}

	array1[ hole ] = tmp;

}

template <class Comparable>

void BinaryHeap<Comparable>::decreaseElements( int t)
{
	for (int i = 0; i < currentSize; i++)
	{
		array1[i+1] - t;
	}
}