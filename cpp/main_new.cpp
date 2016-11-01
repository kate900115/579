#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include "wire.h"
#include <string>

using namespace std;

bool PODEM(Wire* W);
void Objective(Gate* G, Wire* W);
void Initialize();

vector<Wire*> CWire;
vector<Gate*> CGate;

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
				Wire* NewInput = new Wire(word, INPUT, 0, X);
				CWire.push_back(NewInput);
				InputSize++;
			}
	   	}

		else if (word=="output")
		{
			while (sstr >> word)
			{
				Wire* NewOutput = new Wire(word, OUTPUT, 0, X);
				CWire.push_back(NewOutput);
			}
		}

		else if (word=="wire")
		{
			while (sstr >> word)
			{
				Wire* NewWire = new Wire(word, WIRE, 0, X);
				CWire.push_back(NewWire);

			}	
		}
		else if (word=="dff")
		{
			string dff_name;
			string dff_output;
			string dff_input;
			sstr >> dff_name;
			sstr >> dff_output;
			sstr >> dff_input;

	
			vector<string> d_inputs;
			d_inputs.push_back(dff_input);

			Wire* DffOutput;
			vector<Wire*> DffInputs;

			for (unsigned i=0; i<CWire.size(); i++)
			{
				if (CWire[i]->GetWireName()==dff_input)
				{
					DffInputs.push_back(CWire[i]);
					
				}
				if (CWire[i]->GetWireName()==dff_output)
				{
					DffOutput = CWire[i];
				}
			}
		
			Gate* NewDFF = new Gate(dff_name, DFF, DffInputs, DffOutput);
			CGate.push_back(NewDFF);
			
			for (unsigned i=0; i<CWire.size(); i++)
			{
				if(CWire[i]->GetWireName() == dff_output)
				{
					CWire[i]->SetFanIn(NewDFF);	
				}
				if (CWire[i]->GetWireName() == dff_input)
				{
					CWire[i]->AddFanOut(NewDFF);
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

			Wire* NotOutput;
			vector<Wire*> NotInputs;

			for (unsigned i=0; i<CWire.size(); i++)
			{
				if (CWire[i]->GetWireName()==not_input)
				{
					NotInputs.push_back(CWire[i]);
					
				}
				if (CWire[i]->GetWireName()==not_output)
				{
					NotOutput = CWire[i];
				}
			}
			
			Gate* NewNot = new Gate(not_name, NOT, NotInputs, NotOutput);
			CGate.push_back(NewNot);

			for (unsigned i=0; i<CWire.size(); i++)
			{
				if(CWire[i]->GetWireName() == not_output)
				{
					CWire[i]->SetFanIn(NewNot);	
				}
				if (CWire[i]->GetWireName() == not_input)
				{
					CWire[i]->AddFanOut(NewNot);
				}
			}
		}
		else if (word=="and")
		{
			string and_name;
			sstr >> and_name;

			string and_output;
			sstr >> and_output;
		
			string and_input;

			Wire* AndOutput;
			vector<Wire*> AndInputs;

			for (unsigned i=0; i<CWire.size(); i++)
			{
				if(CWire[i]->GetWireName()==and_output)
				{
					AndOutput = CWire[i];
					break;
				}
			}

			while (sstr >> and_input)
			{	
				for (unsigned i=0; i<CWire.size(); i++)
				{
					if (CWire[i]->GetWireName()==and_input)
					{
						AndInputs.push_back(CWire[i]);
					}
				}
			}
				
			int size = AndInputs.size();


			Gate* NewAnd = new Gate(and_name, AND, AndInputs, AndOutput);
			CGate.push_back(NewAnd);
			for (unsigned j=0; j<2; j++)
			{
				and_input = AndInputs[j]->GetWireName();
				for (unsigned i=0; i<CWire.size(); i++)
				{
					if(CWire[i]->GetWireName() == and_input)
					{
						CWire[i]->AddFanOut(NewAnd);	
					}
				}
			}
					
			for (unsigned i=0; i<CWire.size(); i++)
			{
				if (CWire[i]->GetWireName() == and_output)
				{

					CWire[i]->SetFanIn(NewAnd);
					//cout<<CWire[i]->GetFanIn()->GetGateName()<<endl;
				}
			}
		}
		else if (word=="or")
		{
			string or_name;
			sstr >> or_name;

			string or_output;
			sstr >> or_output;
		
			string or_input;

			Wire* OrOutput;
			vector<Wire*> OrInputs;

			for (unsigned i=0; i<CWire.size(); i++)
			{
				if(CWire[i]->GetWireName()==or_output)
				{
					OrOutput = CWire[i];
					break;
				}
			}

			while (sstr >> or_input)
			{	
				for (unsigned i=0; i<CWire.size(); i++)
				{
					if (CWire[i]->GetWireName()==or_input)
					{
						OrInputs.push_back(CWire[i]);
					}
				}
			}
				
			int size = OrInputs.size();

			Gate* NewOr = new Gate(or_name, OR, OrInputs, OrOutput);
			CGate.push_back(NewOr);
			for (unsigned j=0; j<2; j++)
			{
				or_input = OrInputs[j]->GetWireName();
				for (unsigned i=0; i<CWire.size(); i++)
				{
					if(CWire[i]->GetWireName() == or_input)
					{
						CWire[i]->AddFanOut(NewOr);	
					}
				}
			}
					
			for (unsigned i=0; i<CWire.size(); i++)
			{
				if (CWire[i]->GetWireName() == or_output)
				{
					CWire[i]->SetFanIn(NewOr);
					//cout<<CWire[i]->GetFanIn()->GetGateName()<<endl;
				}
			}
		}
		else if (word=="nor")
		{
			string nor_name;
			sstr >> nor_name;

			string nor_output;
			sstr >> nor_output;

			string nor_input;
			
			Wire* NorOutput;
			vector<Wire*> NorInputs;

			for (unsigned i=0; i<CWire.size(); i++)
			{
				if(CWire[i]->GetWireName()==nor_output)
				{
					NorOutput = CWire[i];
					break;
				}
			}

			while (sstr >> nor_input)
			{	
				for (unsigned i=0; i<CWire.size(); i++)
				{
					if (CWire[i]->GetWireName()==nor_input)
					{
						NorInputs.push_back(CWire[i]);
					}
				}
			}
				
			int size = NorInputs.size();


			Gate* NewNor = new Gate(nor_name, NOR, NorInputs, NorOutput);
			CGate.push_back(NewNor);
			for (unsigned j=0; j<size; j++)
			{
				nor_input = NorInputs[j]->GetWireName();
				for (unsigned i=0; i<CWire.size(); i++)
				{
					if(CWire[i]->GetWireName() == nor_input)
					{
						CWire[i]->AddFanOut(NewNor);	
					}
				}
			}
					
			for (unsigned i=0; i<CWire.size(); i++)
			{
				if (CWire[i]->GetWireName() == nor_output)
				{
					CWire[i]->SetFanIn(NewNor);
					cout<<CWire[i]->GetFanIn()->GetGateName()<<endl;
				}
			}
		}
		else if (word=="nand")
		{
			string nand_name;
			sstr >> nand_name;

			string nand_output;
			sstr >> nand_output;
		
			string nand_input;

			Wire* NandOutput;
			vector<Wire*> NandInputs;

			for (unsigned i=0; i<CWire.size(); i++)
			{
				if(CWire[i]->GetWireName()== nand_output)
				{
					NandOutput = CWire[i];
					break;
				}
			}

			while (sstr >> nand_input)
			{	
				for (unsigned i=0; i<CWire.size(); i++)
				{
					if (CWire[i]->GetWireName()==nand_input)
					{
						NandInputs.push_back(CWire[i]);
					}
				}
			}
				
			int size = NandInputs.size();


			Gate* NewNand = new Gate(nand_name, NAND, NandInputs, NandOutput);
			CGate.push_back(NewNand);
			for (unsigned j=0; j<size; j++)
			{
				nand_input = NandInputs[j]->GetWireName();
				for (unsigned i=0; i<CWire.size(); i++)
				{
					if(CWire[i]->GetWireName() == nand_input)
					{
						CWire[i]->AddFanOut(NewNand);	
					}
				}
			}
					
			for (unsigned i=0; i<CWire.size(); i++)
			{
				if (CWire[i]->GetWireName() == nand_output)
				{
					CWire[i]->SetFanIn(NewNand);
					cout<<CWire[i]->GetFanIn()->GetGateName()<<endl;
				}
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
		CGate[i]->PrintGate();
		for (int j=0; j<CGate[i]->GetInputSize(); j++)
		{
			cout<<"input"<<j<<"="<<(CGate[i]->GetInputs())[j]->GetWireName()<<", ";
		}
		cout<<"output = "<<CGate[i]->GetOutput()->GetWireName()<<endl;
	}
	//print all the Wire read from input file
	int WireSize = CWire.size();
	for (int i=0; i<WireSize; i++)
	{
		CWire[i]->PrintWire();
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
		CWire[i]->SetStack(true,D);


		if(PODEM(CWire[i])==true)
		{	
			TestNumber++;
			cout<<"Wire "<<CWire[i]->GetWireName()<<"/0 has test vector"<<endl;
		}
		else
		{
			cout<<"Wire "<<CWire[i]->GetWireName()<<"/0 has no test vector"<<endl;
		}

		//initialize and set s-a-1 fault
		Initialize();
		CWire[i]->SetStack(true,DNOT);
		if(PODEM(CWire[i])==true)
		{	
			TestNumber++;
			cout<<"Wire "<<CWire[i]->GetWireName()<<"/1 has test vector"<<endl;
		}
		else
		{
			cout<<"Wire "<<CWire[i]->GetWireName()<<"/1 has no test vector"<<endl;
		}		
	}
	return 0;
}

bool PODEM(Wire* W)
{
	//if Stack at fault is at primary output
	//we don't need to generate test vector
	Wire* CurrentWire = W;	
		
	if (CurrentWire->GetWireType()==OUTPUT)
	{
		for (int j=0; j<InputSize; j++)
		{
			TestVector[j]=0;
		}
		return true;
	}
	else
	{
		//find D-frontier
		vector<Gate*> DFront = CurrentWire->GetFanOut();

		//Implication
		//to the front
		for (int i=0; i<DFront.size(); i++)
		{
			if (DFront[i]->GetVisited() == false)
			{
				if (DFront[i]->GetGateType()==NOT)
				{
					Gate* CurrentGate = DFront[i];
					CurrentGate->SetVisited(true);
					CurrentGate->GetOutput()->SetValue(LookUpTable(CurrentGate));

					DFront.erase(DFront.begin()+i);
					DFront.insert(DFront.end(), CurrentGate->GetOutput()->GetFanout().begin(), CurrentGate->GetOutput()->GetFanout().end());

				}
			}
		}

		//pick up a gate to do objective()
		for (int i=0; i<DFront.size(); i++)
		{
			if (DFront[i]->GetVisited() == false)
			{
				Objective(DFront[i],CurrentWire);
				break;
			}
		}

	}
	return true;
}

void Initialize()
{
	int size = CWire.size();
	for (int i=0; i<size; i++)
	{
		CWire[i]->initialize();
	}
}


void Objective(Gate* G, Wire* W)
{
	if (G->GetGateType()==AND)
	{
		G->GetOutput()->SetValue(W->GetValue());
		for (unsigned i=0; i<G->GetInputs().size(); i++)
		{
			if (G->GetInputs()[i]!=W)
			{
				G->GetInputs()[i]->SetValue(ONE);
			}
		}
	}
	else if (G->GetGateType()==OR)
	{
		G->GetOutput()->SetValue(W->GetValue());
		for (unsigned i=0; i<G->GetInputs().size(); i++)
		{
			if (G->GetInputs()[i]!=W)
			{
				G->GetInputs()[i]->SetValue(ZERO);
			}
		}
	}
	else if (G->GetGateType()==NAND)
	{
		if (W->GetValue()==D)
		{
			G->GetOutput()->SetValue(DNOT);
		}
		else if (W->GetValue()==DNOT)
		{
			G->GetOutput()->SetValue(D);
		}
		for (unsigned i=0; i<G->GetInputs().size(); i++)
		{
			if (G->GetInputs()[i]!=W)
			{
				G->GetInputs()[i]->SetValue(ONE);
			}
		}
	}
	else if (G->GetGateType()==NOR)
	{
		if (W->GetValue()==D)
		{
			G->GetOutput()->SetValue(DNOT);
		}
		else if (W->GetValue()==DNOT)
		{
			G->GetOutput()->SetValue(D);
		}
		for (unsigned i=0; i<G->GetInputs().size(); i++)
		{
			if (G->GetInputs()[i]!=W)
			{
				G->GetInputs()[i]->SetValue(ZERO);
			}
		}
	}
}


void Backtrace()
{

}


DType LookUpTable(Gate* G)
{
	if (G->GetGateType()==NOT)
	{
		DType InputValue = G->GetInputs()[0]->GetValue();
		if (InputValue == D)
		{return DNOT;}
		if (InputValue == DNOT)
		{return D;}
		if (InputValue == ONE)
		{return ZERO;}
		if (InputValue == ZERO)
		{return ONE;}
		if (InputValue == X)
		{return X;}
	}
	else //need change
	{return X;}
}
