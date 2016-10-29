#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "gate.h"
#include <string>

using namespace std;

int main(int argc, char **argv)
{
	vector<Wire> CWire;
	vector<Gate> CGate;
	

	//read inputfile
	string FileName = string(argv[1]);
	ifstream inFile;
	inFile.open(FileName);
	
	if (!inFile.good())
	{
		cout << "ERROR in read_input() - cannot open " << FileName << " for reading." << endl;
	}
	

	//store the inputfile into the circuit data structure
	while (getline(inFile, line))
	{
		stringstream sstr;
	    	// skip empty lines
	    	if (line.length() == 0) break;
	    
	    	sstr << line;
	    	string word;
	    	vector<string> words;
    		
		//to get the type
		sstr >>word;
		switch (word)
		case "input":
		{
			while (sstr >> word)
			{
				Wire NewInput(word, INPUT, 0, X);
				CWire.push_back(NewInput);
			}
	    	}

		case "output":
		{
			while (sstr >> word)
			{
				Wire NewOutput(word, OUTPUT, 0, X);
				CWire.push_back(NewOutput);
			}
		}

		case "wire" :
		{
			while (sstr >> word)
			{
				Wire NewWire(word, WIRE, 0, X);
				CWire.push_back(NewWire);
			}	
		}
		case "dff" :
		{
			string dff_name;
			string dff_input;
			string dff_output;

			sstr >> dff_name;
			sstr >> dff_input;
			sstr >> dff_output;

			vector<string> d_inputs;
			d_inputs.push_back(dff_input);
			
			Gate NewDFF(dff_name, DFF, d_inputs, dff_output);
		}
		case "not" :
		{
			string not_name;
			string not_input;
			string not_output;

			sstr >> not_name;
			sstr >> not_input;
			sstr >> not_input;
			
			vector<sting> n_inputs;
			d_inputs.push_back(not_input);
			
			Gate NewNot(not_name, NOT, n_inputs, not_output);
		}
		case "and" :
		{
			string and_name;
		}
	    	case "or" :
		{

		}
	    	case "nor" :
		{

		}
		case "nand" :
		{

		}
		
		// parse non-empty lines only
		
	}
  
	cout << "file " << filename << " successfully read." << endl;
	inFile.close();


	return 0;
}

