#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "gate.h"
#include "wire.h"
#include <string>

using namespace std;

int main(int argc, char **argv)
{
	vector<Wire> CWire;
	vector<Gate> CGate;

	//read inputfile
	string FileName = string(argv[1]);
	ifstream inFile;
	inFile.open(FileName.c_str());
	
	
	if (!inFile.good())
	{
		cout << "ERROR in read_input() - cannot open " << FileName << " for reading." << endl;
	}
	
	string line;
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
		if (word=="input")
		{
			while (sstr >> word)
			{
				Wire NewInput(word, INPUT, 0, X);
				CWire.push_back(NewInput);
			}
	    	}

		else if (word=="output")
		{
			while (sstr >> word)
			{
				Wire NewOutput(word, OUTPUT, 0, X);
				CWire.push_back(NewOutput);
			}
		}

		else if (word=="wire")
		{
			while (sstr >> word)
			{
				Wire NewWire(word, WIRE, 0, X);
				CWire.push_back(NewWire);
			}	
		}
		else if (word=="dff")
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
			CGate.push_back(NewDFF);
		}
		else if	(word=="not")
		{
			string not_name;
			string not_input;
			string not_output;
			sstr >> not_name;
			sstr >> not_input;
			sstr >> not_output;
		
			vector<string> n_inputs;
			n_inputs.push_back(not_input);
			
			Gate NewNot(not_name, NOT, n_inputs, not_output);
			CGate.push_back(NewNot);
		}
		else if (word=="and")
		{
			string and_name;
			sstr >> and_name;
		
			vector<string> and_inputs;
			string and_input;
			while (sstr >> and_input)
			{	
				and_inputs.push_back(and_input);
			}
				
			string and_output = and_inputs.back();
			and_inputs.pop_back();
			int size = and_inputs.size();
			switch (size)
			{
				case (2):
				{
					Gate NewAnd(and_name, AND2, and_inputs, and_output);
					CGate.push_back(NewAnd);
				}
				case (3):
				{			
					Gate NewAnd(and_name, AND3, and_inputs, and_output);
					CGate.push_back(NewAnd);
				}
				case (4):
				{			
					Gate NewAnd(and_name, AND4, and_inputs, and_output);
					CGate.push_back(NewAnd);
				}
				default: ;
			}
		}
		else if (word=="or")
		{
			string or_name;
			sstr >> or_name;
		
			vector<string> or_inputs;
			string or_input;
			while (sstr >> or_input)
			{	
				or_inputs.push_back(or_input);
			}
			
			string or_output = or_inputs.back();
			or_inputs.pop_back();
			int size = or_inputs.size();
			switch (size)
			{
				case (2):
				{
					Gate NewOr(or_name, OR2, or_inputs, or_output);
					CGate.push_back(NewOr);
				}
				case (3):	
				{
					Gate NewOr(or_name, OR3, or_inputs, or_output);
					CGate.push_back(NewOr);
				}
				case (4):	
				{				
					Gate NewOr(or_name, OR4, or_inputs, or_output);
					CGate.push_back(NewOr);
				}
				default: ;
			}
		}
		else if (word=="nor")
		{
			string nor_name;
			sstr >> nor_name;
			
			vector<string> nor_inputs;
			string nor_input;
			while (sstr >> nor_input)
			{	
				nor_inputs.push_back(nor_input);
			}
				
			string nor_output = nor_inputs.back();
			nor_inputs.pop_back();

			int size = nor_inputs.size();
			switch (size)
			{
				case (2):
				{
					Gate NewNor(nor_name, NOR2, nor_inputs, nor_output);
					CGate.push_back(NewNor);
				}
				case (3):
				{				
					Gate NewNor(nor_name, NOR3, nor_inputs, nor_output);
					CGate.push_back(NewNor);
				}
				case (4):
				{
					Gate NewNor(nor_name, NOR4, nor_inputs, nor_output);
					CGate.push_back(NewNor);
				}
				default: ;
			}
		}
		else if (word=="nand")
		{
			string nand_name;
			sstr >> nand_name;
		
			vector<string> nand_inputs;
			string nand_input;
			while (sstr >> nand_input)
			{	
				nand_inputs.push_back(nand_input);
			}
				
			string nand_output = nand_inputs.back();
			nand_inputs.pop_back();
			int size = nand_inputs.size();
			switch (size)
			{
				case (2):
				{
					Gate NewNand(nand_name, NAND2, nand_inputs, nand_output);
					CGate.push_back(NewNand);
				}
				case (3):
				{
					Gate NewNand(nand_name, NAND3, nand_inputs, nand_output);
					CGate.push_back(NewNand);
				}
				case (4):
				{				
					Gate NewNand(nand_name, NAND4, nand_inputs, nand_output);
					CGate.push_back(NewNand);
				}
				default: ;
			}
		}
		
	}
  
	cout << "file " << FileName << " successfully read." << endl;
	inFile.close();

	int GateSize=CGate.size();
	for (int i=0; i<GateSize; i++)
	{
		CGate[i].PrintGate();
	}

	int WireSize = CWire.size();
	for (int i=0; i<WireSize; i++)
	{
		CWire[i].PrintWire();
	}


	return 0;
}

