#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "gate.h"
#include <string>

using namespace std;

int main(int argc, char **argv)
{
	//read inputfile
	string FileName = string(argv[1]);
	ifstream inFile;
	inFile.open(FileName);
	
	if (!inFile.good())
	{
		cout << "ERROR in read_input() - cannot open " << FileName << " for reading." << endl;
	}
	
	//store the inputfile into the circuit data structure

}

