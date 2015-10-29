/*
Author:           James Keats
Class:            CSI-281-02
Assignment:       PA6
Date Assigned:    10/22/2015
Due Date:         10/29/2015 @ 9:30AM

Description:
A program for QUICKLY encrypting a given file based on a code word file.

Certification of Authenticity:
I certify that this is entirely my own work, except where I have given fully-documented references
to the work of others. I understand the definition and consequences of plagiarism and acknowledge
that the assessor of this assignment may, for the purpose of assessing this assignment:
- Reproduce this assignment and provide a copy to another member of academic staff; and/or
- Communicate a copy of this assignment to a plagiarism checking service (which may then
retain a copy of this assignment on its database for the purpose of future plagiarism checking)
*/
#include "header.h"
#include <string>
#include <fstream>
#include <iomanip>
#include "timerSystem.h"


/*********************************************************************************************
*  Purpose: To encrypt an entire file based on the AlphabetTree which has been pre-loaded
*			with information.
*      Pre: Handed an initialized and populated AlphabetTree, and two initialized file names,
*			one for the source and the other for the output.
*	  Post: Source is read in, encrypted, and outputted to target. Returns the amount of time
*			that this took, or -1 if either file couldn't be opened.
*********************************************************************************************/
double encryptFile(AlphabetTree &treeRoot, const string &source, const string &target)
{
	ifstream fin(source);
	ofstream fout(target);
	string tmp = "";
	char *returnedValue;

	double time;
	TimerSystem timer;

	if (!fin.good() || !fout.good())
	{
		fin.close();
		fout.close();
		return -1.0;
	}

	timer.startClock();
	fin >> tmp;
	do
	{
		//turns out, converting to c_string is significantly faster
		returnedValue = treeRoot.doSearchIterative(tmp.c_str(), tmp.length());

		fout << returnedValue << " ";
		fin >> tmp;

	} while (!fin.eof());

	time = timer.getTime();

	fin.close();
	fout.close();

	return time;
}


/*********************************************************************************************
*  Purpose:	Get the two file names from the user, or if they want to exit.
*      Pre:	Input and output are declared in calling function.
*	  Post:	Returns false if the user entered the EXIT_KEYWORD. Otherwise, true.
*********************************************************************************************/
bool getFileNames(string &input, string &output)
{
	input = "";
	output = "";

	while (input == output)
	{
		cout << "\tEnter source filename: ";
		getline(cin, input);

		if (input == EXIT_KEYWORD)
		{
			return false;
		}


		cout << "\tEnter target filename: ";
		getline(cin, output);

		if (input == output)
		{
			cout << "\a\n\t***Target cannot equal source!***" << endl << endl;
		} 
	}

	return true;
}


/*********************************************************************************************
*  Purpose: Load the information in the dictionary/code file to the root AlphabetTree.
*      Pre: rootNode is declared in calling function.
*	  Post: rootNode is populated with all of the codes. Returns the amount of time this took,
*			or -1 if the CODES_FILE could not be opened.
*********************************************************************************************/
double loadData(AlphabetTree & rootNode)
{
	string tmpWord;
	string tmpCode;
	TimerSystem timer;
	double time;

	ifstream fin;
	fin.open(CODES_FILE);

	if (!fin.good())
	{
		fin.close();
		return -1.0;
	}

	timer.startClock();

	fin >> tmpWord >> tmpCode;
	while (!fin.eof())
	{
		rootNode.addWord(tmpWord, tmpCode);
		fin >> tmpWord >> tmpCode;
	}

	time = timer.getTime();

	fin.close();

	return time;
}


/*********************************************************************************************
*  Purpose: Loads the code dictionary, then prompts the user for files to encrypt.
*      Pre: None
*	  Post: None
*********************************************************************************************/
void mainMenu()
{
	bool shutdown = false;
	string source, target;
	AlphabetTree treeRoot;
	double time;

	time = loadData(treeRoot);
	if (time < 0)
	{
		cout << "The code file could not be loaded!" << endl;
		return;
	}

	cout << fixed << showpoint << setprecision(4)
		 << "\tLoading time: " << time << " seconds" << endl << endl;

	cout << setprecision(8);

	while (shutdown == false)
	{
		if (getFileNames(source, target) == false) //returns false if user entered EXIT_KEYWORD
		{
			shutdown = true;
		}
		else
		{
			time = encryptFile(treeRoot, source, target);

			if (time < 0)
				cout << "\n\t\tEncryption failed. File could not be opened." << endl << endl;
			else
				cout << "\n\t\tEncryption complete!\n\t\t\tTime: " << time << endl << endl;
		}
	}
}