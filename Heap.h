#ifndef HEAP_H
#define HEAP_H
#include <string>
#include <vector>
using namespace std;

struct Job
{
	int JobId;
	int deadline;
	bool operator<(const Job & j) 
	{
		if (deadline < j.deadline)
		{
			return true;
		}

		else
		{
			return false;
		}
	}  
	bool operator>(const Job & j) 
	{
		if (deadline > j.deadline)
		{
			return true;
		}
		else
		{
			return false;
		}
	}  
	Job(int j, int d)
	{
		JobId = j;
		deadline = d;
	}
};

struct Worker
{
	int WorkerId;
	Job * assignedJob;
	bool operator<(const Worker & w)
	{
		if (assignedJob->deadline < w.assignedJob->deadline)
		{
			return true;
		}
		else if (assignedJob->deadline == w.assignedJob->deadline && WorkerId < w.WorkerId)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator>(const Worker & w) 
	{
		if (assignedJob->deadline > w.assignedJob->deadline)
		{
			return true;
		}
		else if (assignedJob->deadline == w.assignedJob->deadline && WorkerId > w.WorkerId)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void operator-(const int & d) 
	{
		this->assignedJob->deadline = this->assignedJob->deadline - d;
	}
	Worker()
	{
	}
	Worker(int w, Job * j)
	{
		WorkerId = w;
		assignedJob = j;
	}
};



template <class Comparable>

class BinaryHeap

{

public:

	BinaryHeap( int capacity = 100 );

	bool isEmpty( ) const;

	bool isFull( ) const;

	//const Comparable & findMin( ) const;

	void insert(Comparable & x );

	Comparable getMin( );

	void deleteMin();

	int currentSize; // Number of elements in heap

	vector<Comparable> array1 ; // The heap array

	void buildHeap(vector<Comparable> Input);

	void percolateDown( int hole );

	void decreaseElements(int t);

};

#include "Heap.cpp"

#endif

