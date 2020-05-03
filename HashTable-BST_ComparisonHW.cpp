#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include "BinarySearchTree.h"
#include "HashTable.h"
using namespace std;

int main()
{
	string command; string queriedWord; string queriedTranslation; BinarySearchTree<string> maintree;

	//////////////////////////////////////////////////////////////// BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST 
	ifstream program;
	string programname = "dict.txt";
	program.open(programname.c_str());
	string line;
	while (getline(program,line))
	{
		istringstream linestream(line);
		string word1; bool firstWord=true;
		string word2,translation2;
		while (linestream >> word1)
		{
			if (firstWord == true)
			{
				word2 = word1;
				firstWord = false;
			}
			else
			{
				translation2 = translation2 + " " + word1; 
			}
		}
		if (maintree.find(word2) == "-1")
		{
			maintree.insert(word2,translation2); 
		}
		else if (maintree.find(word2) != translation2)
		{
			maintree.findAndUpdate(word2,translation2);
		}
	}
	cout <<	"Building a binary tree for dict.txt..." << endl;
	program.clear();
	program.seekg(0);
	//////////////////////////////////////////////////////////////// BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST BST 

	//////////////////////////////////////////////////////////////// HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE 
	HashTable<string> h1("ItemNotFound",53);
	cout <<	"Building a hashtable for dict.txt..." << endl;	
	int count=0;
	while (getline(program,line))
	{
		istringstream linestream(line);
		string word1; bool firstWord=true;
		string word2,translation2;
		while (linestream >> word1)
		{
			if (firstWord == true)
			{
				word2 = word1;
				firstWord = false;
			}
			else
			{
				translation2 = translation2 + " " + word1; 
			}
		}
		if (h1.find(word2) == "ItemNotFound" || h1.find(word2) != translation2)
		{
			h1.insert(word2,translation2); 
		}
	}
	//////////////////////////////////////////////////////////////// HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE HASHTABLE 

	cout <<	"Running queries in query1.txt..." << endl << endl;
	ifstream queryFile; ofstream resultFile; ofstream resultFile2;
	string queryName = "query1.txt"; string resultName = "ht_result.txt"; string resultName2 = "bst_result.txt";
	queryFile.open(queryName.c_str());
	resultFile.open(resultName.c_str());
	resultFile2.open(resultName2.c_str());
	string line1; int count3=0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto start = std::chrono::high_resolution_clock::now();

	while (getline(queryFile,line1))
	{
		istringstream linestream1(line1);
		string word3;
		while (linestream1 >> word3)
		{
			resultFile2 << word3 << " " << maintree.find(word3) << endl;
			count3++;
		}
	}
	auto time = (std::chrono::high_resolution_clock::now() - start).count();
	float bstTime = time;
	cout << "***********************************************" << endl;
	cout <<	"Benchmark results for Binary Search Tree (BST):" << endl;
	cout << "***********************************************" << endl;
	cout << "Elapsed time: " << bstTime << " ns" <<"\n";
	cout << "Average query time: " << bstTime/count3 << " ns" << endl << endl;
	queryFile.clear();
	queryFile.seekg(0);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	string line2;

	auto start1 = std::chrono::high_resolution_clock::now();
	while (getline(queryFile,line2))
	{
		istringstream linestream2(line2);
		string word4;
		while (linestream2 >> word4)
		{
			resultFile << word4 << " " << h1.find(word4) << endl;
		}
	}
	auto time1 = (std::chrono::high_resolution_clock::now() - start1).count();
	float htTime = time1;
	cout << "***********************************************" << endl;
	cout <<	"Benchmark results for Hash Table:" << endl;
	cout << "***********************************************" << endl;
	cout << "Elapsed time: " << htTime << " ns" << "\n";
	cout << "Average query time: " << htTime/count3 << " ns" << endl << endl;
	float speedup = bstTime/htTime;
	cout << "Speed Up: " << speedup << endl << endl;
	cout <<	"bst" << endl;
	cout << "N time" << endl;
	queryFile.clear();
	queryFile.seekg(0);
	auto start2 = std::chrono::high_resolution_clock::now();
	int tcount=0;
	int ncount=2;
	float timerBST = 0;
	while (tcount < 4097)
	{
		while (getline(queryFile,line1))
		{
			istringstream linestream1(line1);
			string word3;
			while (linestream1 >> word3)
			{
				maintree.find(word3);
				count3++;
			}
		}
		queryFile.clear();
		queryFile.seekg(0);
		if (tcount == ncount)
		{
			auto time2 = (std::chrono::high_resolution_clock::now() - start2).count();
			cout << ncount << " " << time2 << endl;;
			ncount = ncount * 2;
			timerBST = timerBST + time2; 	//Sum of the times of BST 
		}
		tcount++;
	}



		queryFile.clear();
	queryFile.seekg(0);
	cout << "ht" << endl;
	cout << "N time" << endl;
	float timerHT = 0;
	auto start3 = std::chrono::high_resolution_clock::now();
	int tcount1=0;
	int ncount1=2;
	while (tcount1 < 4097)
	{
		while (getline(queryFile,line1))
		{
			istringstream linestream1(line1);
			string word3;
			while (linestream1 >> word3)
			{
				h1.find(word3);
				count3++;
			}
		}
		queryFile.clear();
		queryFile.seekg(0);
		if (tcount1 == ncount1)
		{
			auto time3 = (std::chrono::high_resolution_clock::now() - start3).count();
			cout << ncount1 << " " << time3 << endl;;
			ncount1 = ncount1 * 2;
			timerHT = timerHT + time3; 	//Sum of the times of HT 
		}
		tcount1++;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////LAST PART
	cout << endl;
	cout << "BST TIME/HT TIME " << timerBST/timerHT  << endl;

	///WHICH ONE IS FASTER?
	if (timerBST > timerHT)
	{
		cout << "HashTable is faster! (Look at the last part of my code)" << endl; 
	}
	else
	{
		cout << "BST is faster! (Look at the last part of my code)" << endl; 
	}
	return 0;
}