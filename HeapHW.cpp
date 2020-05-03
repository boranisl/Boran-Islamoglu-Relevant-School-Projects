#include <iostream>
#include <string>
#include "Heap.h"
#include <fstream>
#include <sstream>
#include "strutils.h"

using namespace std;

int main()
{
	ifstream program;
	string programname;
	cout <<	"input: ";
	cin >> programname;
	program.open(programname.c_str());
	string line; int count=-1; int jcount = 0;
	vector<Worker> workerArray;
	vector<Job> jobArray;
	Job j0(0,0); jobArray.push_back(j0);
	int numWorkers=0; int numJobs=0;
	while (getline(program,line))
	{
		istringstream linestream(line);
		string word1; 
		while (linestream >> word1)
		{
			if (count == -1)
			{
				numWorkers = atoi(word1);
				count++; Worker e1(0,nullptr);
				int count2 = 1; workerArray.push_back(e1);
				while (count2 <= numWorkers)
				{
					Worker w1(count2,nullptr);
					workerArray.push_back(w1);
					count2++;
				}
			}
			else if (count == 0)
			{
				numJobs = atoi(word1);
				count++;
			}
			else
			{
				if (count == 1) // ID
				{
					jcount++;
					count++;
				}
				else if (count == 2) //DEADLINE	
				{
					Job j1(jcount,atoi(word1));
					jobArray.push_back(j1);
					count = 1;
				}
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ofstream resultFile;
	string resultName = programname + ".out";
	resultFile.open(resultName.c_str());
	vector<Worker> AssignedWorkers; //
	for (int i = 1; i <= numJobs && i <= numWorkers ; i++) 
	{
		workerArray[i].assignedJob = &jobArray[i];
		AssignedWorkers.push_back(workerArray[i]); //
		resultFile << "Job-" <<jobArray[i].JobId << "->Worker-" << workerArray[i].WorkerId <<	" (Day: 0)" << endl;
	}
	BinaryHeap<Worker> mainheap(numWorkers);
	mainheap.buildHeap(AssignedWorkers); //

	int a = numWorkers + 1; int totaldays = 0; int lastday = 0;
	while (a < jobArray.size())
	{
		lastday = mainheap.getMin().assignedJob->deadline;
		totaldays = totaldays + mainheap.getMin().assignedJob->deadline;
		int w = mainheap.getMin().WorkerId;
		workerArray[w].assignedJob = &jobArray[a];
		mainheap.deleteMin();
		mainheap.decreaseElements(lastday);
		Worker w1 = workerArray[w];
		mainheap.insert(w1);
	    resultFile << "Job-" <<jobArray[a].JobId << "->Worker-" << workerArray[w].WorkerId <<	" (Day:" << totaldays << ")" << endl;
		a++;

	}
	for (int i = 1; i <= mainheap.currentSize ; i)
	{
		lastday = mainheap.getMin().assignedJob->deadline;
		totaldays = mainheap.getMin().assignedJob->deadline + totaldays;
		mainheap.deleteMin();
		mainheap.decreaseElements(lastday);
	}
	resultFile << "All jobs are completed in " << totaldays << " days." << endl;


	return 0;
}