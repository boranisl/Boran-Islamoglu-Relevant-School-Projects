#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "BinarySearchTree.h"

using namespace std;

int main()
{
	string command; string queriedWord; string queriedTranslation; BinarySearchTree<string> maintree;

	//////////////////////////////////////////////////////////////// INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT
	ifstream program;
	string programname = "dictUpdatedEN.txt";
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
	//////////////////////////////////////////////////////////////// INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT INPUT


	cout <<	"***Help***" << endl;
	cout << "Query: 1" << endl;
	cout << "Add new translation: 2" << endl;
	cout << "Add new word: 3" << endl;
	cout << "Exit: 0" << endl;
	while (true)
	{
		cout << "Enter Command: ";
		cin >> command;

		if (command == "1") //Query the word and find the meaning
		{
			cout << "Enter queried word: ";
			cin >> queriedWord;
			if (maintree.find(queriedWord) != "-1")
			{
				cout << queriedWord << "  --->  " << maintree.find(queriedWord) << endl;
			}
			else
			{
				cout <<	"There is no such word in the dictionary!" << endl;
			}
		}
		else if (command == "2") //Add new translation for existing word
		{
			cout <<	"Enter queried word: ";
			cin >> queriedWord;
			if (maintree.find(queriedWord) != "-1"){
				cout << "Enter the new translation: ";
				cin >> queriedTranslation;
				maintree.findAndUpdate(queriedWord,queriedTranslation);
				cout << "***UPDATED***" << endl;
			}
			else
			{
				cout <<	"There is no such word in the dictionary!" << endl;
			}
		}
		else if (command == "3") // Add new word and translation of the word.
		{
			cout <<	"Enter New Word: ";
			cin >> queriedWord;
			if (maintree.find(queriedWord) == "-1")
			{
				cout << "Enter Translations: ";
				cin >> queriedTranslation;
				maintree.insert(queriedWord,queriedTranslation);
			}
			else
			{
				cout << "Word already exists, if you want to add new translation please use command 2." << endl;
			}
		}
		else if (command == "0") //Exit the program.
		{
			cout << "Bye..." << endl;
			break;
		}
		else // Invalid Command
		{

		}
	}
	return 0;
}