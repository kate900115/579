#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include "wire.h"
#include <string>

using namespace std;

bool PODEM();
void Initialize();

vector<Wire> CWire;
vector<Gate> CGate;

stack<Wire> BacktraceWire;

vector<bool> TestVector;

int InputSize=0;

int main(int argc, char **argv)
{
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
				InputSize++;
			}	
		}
		else if (word=="dff")
		{
			string dff_name;
			string dff_output;
			string dff_input;
			sstr >> dff_name;
			sstr >> dff_input;
			sstr >> dff_output;
	
			vector<string> d_inputs;
			d_inputs.push_back(dff_input);
		
			Gate NewDFF(dff_name, DFF, d_inputs, dff_output);
			CGate.push_back(NewDFF);
			
			for (unsigned i=0; i<CWire.size(); i++)
			{
				if(CWire[i].GetWireName() == dff_output)
				{
					CWire[i].SetFanIn(&NewDFF);	
				}
				if (CWire[i].GetWireName() == dff_input)
				{
					CWire[i].AddFanOut(&NewDFF);
				}
			}
		}
		else if	(word=="not")
		{
			string not_name;
			string not_input;
			string not_output;
			sstr >> not_name;
			sstr >> not_output;
			sstr >> not_input;
		
			vector<string> n_inputs;
			n_inputs.push_back(not_input);
			
			Gate NewNot(not_name, NOT, n_inputs, not_output);
			CGate.push_back(NewNot);

			for (unsigned i=0; i<CWire.size(); i++)
			{
				if(CWire[i].GetWireName() == not_output)
				{
					CWire[i].SetFanIn(&NewNot);	
				}
				if (CWire[i].GetWireName() == not_input)
				{
					CWire[i].AddFanOut(&NewNot);
				}
			}
		}
		else if (word=="and")
		{
			string and_name;
			sstr >> and_name;

			string and_output;
			sstr >> and_output;
		
			vector<string> and_inputs;
			string and_input;
			while (sstr >> and_input)
			{	
				and_inputs.push_back(and_input);
			}
				
			int size = and_inputs.size();

			switch (size)
			{
				case (2):
				{
					Gate NewAnd(and_name, AND2, and_inputs, and_output);
					CGate.push_back(NewAnd);
					for (unsigned j=0; j<2; j++)
					{
						and_input = and_inputs[j];
						for (unsigned i=0; i<CWire.size(); i++)
						{
							if(CWire[i].GetWireName() == and_input)
							{
								CWire[i].AddFanOut(&NewAnd);	
							}
						}
					}
					
					for (unsigned i=0; i<CWire.size(); i++)
					{
						if (CWire[i].GetWireName() == and_output)
						{
							CWire[i].SetFanIn(&NewAnd);
						}
					}
					break;
				}
				case (3):
				{			
					Gate NewAnd(and_name, AND3, and_inputs, and_output);
					CGate.push_back(NewAnd);
					for (unsigned j=0; j<3; j++)
					{
						and_input = and_inputs[j];
						for (unsigned i=0; i<CWire.size(); i++)
						{
							if(CWire[i].GetWireName() == and_input)
							{
								CWire[i].AddFanOut(&NewAnd);	
							}
						}
					}
					
					for (unsigned i=0; i<CWire.size(); i++)
					{
						if (CWire[i].GetWireName() == and_output)
						{
							CWire[i].SetFanIn(&NewAnd);
						}
					}
					break;
				}
				case (4):
				{			
					Gate NewAnd(and_name, AND4, and_inputs, and_output);
					CGate.push_back(NewAnd);
					for (unsigned j=0; j<4; j++)
					{
						and_input = and_inputs[j];
						for (unsigned i=0; i<CWire.size(); i++)
						{
							if(CWire[i].GetWireName() == and_input)
							{
								CWire[i].AddFanOut(&NewAnd);	
							}
						}
					}
					
					for (unsigned i=0; i<CWire.size(); i++)
					{
						if (CWire[i].GetWireName() == and_output)
						{
							CWire[i].SetFanIn(&NewAnd);
						}
					}
					break;
				}
				default: ;
			}
		}
		else if (word=="or")
		{
			string or_name;
			sstr >> or_name;

			string or_output;
			sstr >> or_output;
		
			vector<string> or_inputs;
			string or_input;
			while (sstr >> or_input)
			{	
				or_inputs.push_back(or_input);
			}
			
			int size = or_inputs.size();
			switch (size)
			{
				case (2):
				{
					Gate NewOr(or_name, OR2, or_inputs, or_output);
					CGate.push_back(NewOr);
					for (unsigned j=0; j<2; j++)
					{
						or_input = or_inputs[j];
						for (unsigned i=0; i<CWire.size(); i++)
						{
							if(CWire[i].GetWireName() == or_input)
							{
								CWire[i].AddFanOut(&NewOr);	
							}
						}
					}
					
					for (unsigned i=0; i<CWire.size(); i++)
					{
						if (CWire[i].GetWireName() == or_output)
						{
							CWire[i].SetFanIn(&NewOr);
						}
					}
					break;
				}
				case (3):	
				{
					Gate NewOr(or_name, OR3, or_inputs, or_output);
					CGate.push_back(NewOr);
					for (unsigned j=0; j<3; j++)
					{
						or_input = or_inputs[j];
						for (unsigned i=0; i<CWire.size(); i++)
						{
							if(CWire[i].GetWireName() == or_input)
							{
								CWire[i].AddFanOut(&NewOr);	
							}
						}
					}
					
					for (unsigned i=0; i<CWire.size(); i++)
					{
						if (CWire[i].GetWireName() == or_output)
						{
							CWire[i].SetFanIn(&NewOr);
						}
					}
					break;
				}
				case (4):	
				{				
					Gate NewOr(or_name, OR4, or_inputs, or_output);
					CGate.push_back(NewOr);	
					for (unsigned j=0; j<4; j++)
					{
						or_input = or_inputs[j];
						for (unsigned i=0; i<CWire.size(); i++)
						{
							if(CWire[i].GetWireName() == or_input)
							{
								CWire[i].AddFanOut(&NewOr);	
							}
						}
					}
					
					for (unsigned i=0; i<CWire.size(); i++)
					{
						if (CWire[i].GetWireName() == or_output)
						{
							CWire[i].SetFanIn(&NewOr);
						}
					}
					break;
				}
				default: ;
			}
		}
		else if (word=="nor")
		{
			string nor_name;
			sstr >> nor_name;

			string nor_output;
			sstr >> nor_output;
			
			vector<string> nor_inputs;
			string nor_input;
			while (sstr >> nor_input)
			{	
				nor_inputs.push_back(nor_input);
			}

			int size = nor_inputs.size();
			switch (size)
			{
				case (2):
				{
					Gate NewNor(nor_name, NOR2, nor_inputs, nor_output);
					CGate.push_back(NewNor);
					for (unsigned j=0; j<2; j++)
					{
						nor_input = nor_inputs[j];
						for (unsigned i=0; i<CWire.size(); i++)
						{
							if(CWire[i].GetWireName() == nor_input)
							{
								CWire[i].AddFanOut(&NewNor);	
							}
						}
					}
					
					for (unsigned i=0; i<CWire.size(); i++)
					{
						if (CWire[i].GetWireName() == nor_output)
						{
							CWire[i].SetFanIn(&NewNor);
						}
					}
					break;
				}
				case (3):
				{				
					Gate NewNor(nor_name, NOR3, nor_inputs, nor_output);
					CGate.push_back(NewNor);
					for (unsigned j=0; j<3; j++)
					{
						nor_input = nor_inputs[j];
						for (unsigned i=0; i<CWire.size(); i++)
						{
							if(CWire[i].GetWireName() == nor_input)
							{
								CWire[i].AddFanOut(&NewNor);	
							}
						}
					}
					
					for (unsigned i=0; i<CWire.size(); i++)
					{
						if (CWire[i].GetWireName() == nor_output)
						{
							CWire[i].SetFanIn(&NewNor);
						}
					}
					break;
				}
				case (4):
				{
					Gate NewNor(nor_name, NOR4, nor_inputs, nor_output);
					CGate.push_back(NewNor);
					for (unsigned j=0; j<4; j++)
					{
						nor_input = nor_inputs[j];
						for (unsigned i=0; i<CWire.size(); i++)
						{
							if(CWire[i].GetWireName() == nor_input)
							{
								CWire[i].AddFanOut(&NewNor);	
							}
						}
					}
					
					for (unsigned i=0; i<CWire.size(); i++)
					{
						if (CWire[i].GetWireName() == nor_output)
						{
							CWire[i].SetFanIn(&NewNor);
						}
					}
					break;
				}
				default: ;
			}
		}
		else if (word=="nand")
		{
			string nand_name;
			sstr >> nand_name;

			string nand_output;
			sstr >> nand_output;
		
			vector<string> nand_inputs;
			string nand_input;
			while (sstr >> nand_input)
			{	
				nand_inputs.push_back(nand_input);
			}
				
			int size = nand_inputs.size();
			switch (size)
			{
				case (2):
				{
					Gate NewNand(nand_name, NAND2, nand_inputs, nand_output);
					CGate.push_back(NewNand);
					for (unsigned j=0; j<2; j++)
					{
						nand_input = nand_inputs[j];
						for (unsigned i=0; i<CWire.size(); i++)
						{
							if(CWire[i].GetWireName() == nand_input)
							{
								CWire[i].AddFanOut(&NewNand);	
							}
						}
					}
					
					for (unsigned i=0; i<CWire.size(); i++)
					{
						if (CWire[i].GetWireName() == nand_output)
						{
							CWire[i].SetFanIn(&NewNand);
						}
					}
					break;
				}
				case (3):
				{
					Gate NewNand(nand_name, NAND3, nand_inputs, nand_output);
					CGate.push_back(NewNand);
					for (unsigned j=0; j<3; j++)
					{
						nand_input = nand_inputs[j];
						for (unsigned i=0; i<CWire.size(); i++)
						{
							if(CWire[i].GetWireName() == nand_input)
							{
								CWire[i].AddFanOut(&NewNand);	
							}
						}
					}
					
					for (unsigned i=0; i<CWire.size(); i++)
					{
						if (CWire[i].GetWireName() == nand_output)
						{
							CWire[i].SetFanIn(&NewNand);
						}
					}
					break;
				}
				case (4):
				{				
					Gate NewNand(nand_name, NAND4, nand_inputs, nand_output);
					CGate.push_back(NewNand);
					for (unsigned j=0; j<4; j++)
					{
						nand_input = nand_inputs[j];
						for (unsigned i=0; i<CWire.size(); i++)
						{
							if(CWire[i].GetWireName() == nand_input)
							{
								CWire[i].AddFanOut(&NewNand);	
							}
						}
					}
					
					for (unsigned i=0; i<CWire.size(); i++)
					{
						if (CWire[i].GetWireName() == nand_output)
						{
							CWire[i].SetFanIn(&NewNand);
						}
					}
					break;
				}
				default: ;
			}
		}
		
	}
  
	cout << "file " << FileName << " successfully read." << endl;
	inFile.close();
	
	/*--------------------for test--------------------*/
	//print all the Gate read from input file 
	int GateSize=CGate.size();
	for (int i=0; i<GateSize; i++)
	{
		CGate[i].PrintGate();
	}
	//print all the Wire read from input file
	int WireSize = CWire.size();
	for (int i=0; i<WireSize; i++)
	{
		CWire[i].PrintWire();
	}
	/*--------------------for test--------------------*/


	int TestNumber = 0;
	// PODEM
	// for each wire we will generate 
	// s-a-0 fault and s-a-1 fault
	for (int i=0; i<WireSize; i++)
	{
		//intialize and set s-a-0 fault
		Initialize();
		CWire[i].SetStack(true,D);


		if(PODEM()==true)
		{	
			TestNumber++;
			cout<<"Wire "<<CWire[i].GetWireName()<<"/0 has test vector"<<endl;
		}
		else
		{
			cout<<"Wire "<<CWire[i].GetWireName()<<"/0 has no test vector"<<endl;
		}

		//initialize and set s-a-1 fault
		Initialize();
		CWire[i].SetStack(true,DNOT);
		if(PODEM()==true)
		{	
			TestNumber++;
			cout<<"Wire "<<CWire[i].GetWireName()<<"/1 has test vector"<<endl;
		}
		else
		{
			cout<<"Wire "<<CWire[i].GetWireName()<<"/1 has no test vector"<<endl;
		}		
	}
	return 0;
}

bool PODEM()
{
	//if Stack at fault is at primary output
	//we don't need to generate test vector
	
	/*
	if (CWire[i].GetWireType()==OUTPUT)
	{
		for (int j=0; j<InputSize; j++)
		{
			TestVector[j]=0;
		}
		return true;
	}
	else
	{
		

	}*/
	return true;
}

void Initialize()
{
	int size = CWire.size();
	for (int i=0; i<size; i++)
	{
		CWire[i].initialize();
	}
}
