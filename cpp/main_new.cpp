#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include "wire.h"
#include <string>

using namespace std;

bool PODEM(Wire* W);
void Initialize();
void ImplyBackward(Gate* G);
bool InputImplyForward();
void ImplyForward(vector<Gate*> Gs);
Wire* Backtrace(Gate* G);
void Objective(Gate* G);
DType LookUpTable(Gate* G);
DType BTLookUpTable(Gate* G);


vector<Wire*> CWire;
vector<Gate*> CGate;
vector<Wire*> InputWires;
vector<Wire*> ComputedInputs;

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
				InputWires.push_back(NewInput);
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
			for (int j=0; j<size; j++)
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
			for (int j=0; j<size; j++)
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
			for (int j=0; j<size; j++)
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
			for (int j=0; j<size; j++)
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

		CWire[i]->SetStuck(true,D);

		//Activate fault
		//Implication to the back
		Gate* DBack = CWire[i]->GetFanIn();
	
		
		if (CWire[i]->GetWireType()!=INPUT)
		{
			ImplyBackward(DBack);
		}
		
		//find D-frontier
		//the remaining problem is if we change 
		//DFront, will it affect the real Fanout of current wire 
		vector<Gate*> DFront = CWire[i]->GetFanOut();
		
		ImplyForward(DFront);
		cout<<"bbbbbbbbbbbbbbbbbbbbbb"<<endl;
		//Do PODEM
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
		CWire[i]->SetStuck(true,DNOT);

		//Activate fault
		//Implication to the back
		DBack = CWire[i]->GetFanIn();
		if (CWire[i]->GetWireType()!=INPUT)
		{
			ImplyBackward(DBack);
		}

		//find D-frontier
		//the remaining problem is if we change 
		//DFront, will it affect the real Fanout of current wire 
		DFront = CWire[i]->GetFanOut();
		ImplyForward(DFront);

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



//functions

bool PODEM(Wire* W)
{
	//if Stuck at fault is at primary output
	//we don't need to generate test vector
	Wire* CurrentWire = W;	
	cout<<"aaaaaaaaaaaaaaaa"<<endl;
		
	if (CurrentWire->GetWireType()==OUTPUT) return true;
	
	//If No DFrontier, untestable
	//return false
	for (unsigned i=0; i<CurrentWire->GetFanOut().size(); i++)
	{
		if (CurrentWire->GetFanOut()[i]->GetOutput()->GetValue()==X)
		{break;}
		else if (CurrentWire->GetFanOut()[i]->GetOutput()->GetValue()==D)
		{break;}
		else if (CurrentWire->GetFanOut()[i]->GetOutput()->GetValue()==DNOT)
		{break;}
		if (i == CurrentWire->GetFanOut().size()-1)
		{return false;}
	}


	//pick up a gate from D-frontier
	//to do objective()
	Gate* FrontierGate = CurrentWire->GetFanOut()[0];
	while (FrontierGate->GetGateType()==NOT)
	{
		if ((FrontierGate->GetInputs())[0]->GetValue()==D) FrontierGate->GetOutput()->SetValue(DNOT);
		else if ((FrontierGate->GetInputs())[0]->GetValue()==DNOT) FrontierGate->GetOutput()->SetValue(D);
		CurrentWire = FrontierGate->GetOutput();
		FrontierGate = CurrentWire->GetFanOut()[0];
	}	

	//objective
	Objective(FrontierGate);
	FrontierGate->GetOutput()->SetValue(LookUpTable(FrontierGate));

	//Backtrace
	Wire* BTResult = Backtrace(FrontierGate);
		
	//if all the Backtrace is done, current wire change 
	//and frontier gate changes
	if(BTResult==NULL)
	{
		FrontierGate = CurrentWire->GetFanOut()[0];
		CurrentWire = FrontierGate->GetOutput();
		

		//if the frontier gate is not gate
		//we need to propagate it further
		//until the frontier is not a Not gate.
		while (FrontierGate->GetGateType()==NOT)
		{
			if ((FrontierGate->GetInputs())[0]->GetValue()==D) FrontierGate->GetOutput()->SetValue(DNOT);
			else if ((FrontierGate->GetInputs())[0]->GetValue()==DNOT) FrontierGate->GetOutput()->SetValue(D);

			FrontierGate = CurrentWire->GetFanOut()[0];
			CurrentWire = FrontierGate->GetOutput();
		}	
	}

	//ImplyForward BTResult to see if there is a contradiction
	if (InputImplyForward())
	{
		if (PODEM(CurrentWire)==true) return true;
	}
	else
	{	
		//implyForward BTResult' to see if there is a contradiction
		if(BTResult->GetValue()==ONE)
			{BTResult->SetValue(ZERO);}
		else if (BTResult->GetValue()==ZERO)
			{BTResult->SetValue(ONE);}
		if(InputImplyForward())
		{
			if (PODEM(CurrentWire)==true) return true;
		}
	}

	//Imply BTResult= X
	BTResult->SetValue(ZERO);
	InputImplyForward();
	return false;
}

void Initialize()
{
	int size = CWire.size();
	for (int i=0; i<size; i++)
	{
		CWire[i]->initialize();
	}
	size = CGate.size();
	for (int i=0; i<size; i++)
	{
		CGate[i]->initialize();
	}
}




void ImplyBackward(Gate* G)
{
	vector<Gate*> gates;
	gates.push_back(G);
	while (gates.size()!=0)
	{
		if (gates.front()->GetGateType()==NOT)
		{
			if (gates.front()->GetOutput()->GetValue()==ONE)
			{
				(gates.front()->GetInputs())[0]->SetValue(ZERO);
				gates.front()->SetVisited(true);
				if (gates.front()->GetInputs()[0]->GetFanIn()->GetVisited()==false)
				{
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==ZERO)
			{
				(gates.front()->GetInputs())[0]->SetValue(ONE);
				gates.front()->SetVisited(true);
				if (gates.front()->GetInputs()[0]->GetFanIn()->GetVisited()==false)
				{
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==D)
			{	
				(gates.front()->GetInputs())[0]->SetValue(ZERO);
				gates.front()->SetVisited(true);
				if (gates.front()->GetInputs()[0]->GetFanIn()->GetVisited()==false)
				{
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==DNOT)
			{	
				(gates.front()->GetInputs())[0]->SetValue(ONE);
				gates.front()->SetVisited(true);
				if (gates.front()->GetInputs()[0]->GetFanIn()->GetVisited()==false)
				{
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}
			}			
		}
		else if (gates.front()->GetGateType()==AND)
		{
			if (gates.front()->GetOutput()->GetValue()==ONE)
			{
				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					(gates.front()->GetInputs())[i]->SetValue(ONE);
				}
				gates.front()->SetVisited(true);

				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					if (gates.front()->GetInputs()[0]->GetFanIn()->GetVisited()==false)
					{
						gates.push_back((gates.front()->GetInputs())[i]->GetFanIn());
					}
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==D)
			{
				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					(gates.front()->GetInputs())[i]->SetValue(ONE);
				}
				gates.front()->SetVisited(true);

				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					if (gates.front()->GetInputs()[0]->GetFanIn()->GetVisited()==false)
					{
						gates.push_back((gates.front()->GetInputs())[i]->GetFanIn());
					}
				}
			}
		}
		else if (gates.front()->GetGateType()==NAND)
		{
			if (gates.front()->GetOutput()->GetValue()==ZERO)
			{
				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					(gates.front()->GetInputs())[i]->SetValue(ONE);
				}
				gates.front()->SetVisited(true);

				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					if (gates.front()->GetInputs()[0]->GetFanIn()->GetVisited()==false)
					{
						gates.push_back((gates.front()->GetInputs())[i]->GetFanIn());
					}
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==DNOT)
			{
				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					(gates.front()->GetInputs())[i]->SetValue(ONE);
				}
				gates.front()->SetVisited(true);

				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					if (gates.front()->GetInputs()[0]->GetFanIn()->GetVisited()==false)
					{
						gates.push_back((gates.front()->GetInputs())[i]->GetFanIn());
					}
				}
			}
		}
		else if (gates.front()->GetGateType()==OR)
		{
			if (gates.front()->GetOutput()->GetValue()==ZERO)
			{
				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					(gates.front()->GetInputs())[i]->SetValue(ZERO);					
				}
				gates.front()->SetVisited(true);

				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					if (gates.front()->GetInputs()[0]->GetFanIn()->GetVisited()==false)
					{
						gates.push_back((gates.front()->GetInputs())[i]->GetFanIn());
					}
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==DNOT)
			{
				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					(gates.front()->GetInputs())[i]->SetValue(ZERO);
				}
				gates.front()->SetVisited(true);

				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					if (gates.front()->GetInputs()[0]->GetFanIn()->GetVisited()==false)
					{
						gates.push_back((gates.front()->GetInputs())[i]->GetFanIn());
					}
				}
			}

		}
		else if (gates.front()->GetGateType()==NOR)
		{
			if (gates.front()->GetOutput()->GetValue()==ONE)
			{
				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					(gates.front()->GetInputs())[i]->SetValue(ZERO);
				}
				gates.front()->SetVisited(true);

				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					if (gates.front()->GetInputs()[0]->GetFanIn()->GetVisited()==false)
					{
						gates.push_back((gates.front()->GetInputs())[i]->GetFanIn());
					}
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==D)
			{
				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					(gates.front()->GetInputs())[i]->SetValue(ZERO);
				}
				gates.front()->SetVisited(true);

				for (int i=0; i<gates.front()->GetInputSize(); i++)
				{
					if (gates.front()->GetInputs()[0]->GetFanIn()->GetVisited()==false)
					{
						gates.push_back((gates.front()->GetInputs())[i]->GetFanIn());
					}
				}
			}
		}
		gates.erase(gates.begin());
	}
}


void ImplyForward(vector<Gate*> Gs)
{
	vector<Gate*> gates = Gs;
	//Implication to the front
	//if there is NOT gate as frontier
	//We can propagate D or D' further
	while (gates.size()!=0)
	{
		if (gates.front()->GetGateType() == NOT)
		{
			gates.front()->GetOutput()->SetValue(LookUpTable(gates.front()));
			if (gates.front()->GetOutput()->GetWireType()!=OUTPUT)
			{
				gates.push_back((gates.front()->GetOutput()->GetFanOut())[0]);
			}
		
			
		}
		gates.erase(gates.begin());	
	}
}


//from primary input to forward gates
//check if there is a contradiction
bool InputImplyForward()
{
	vector<Wire*> wires = InputWires;
	
	while (wires.size()!=0)
	{
		vector<Gate*>fanout = wires.front()->GetFanOut();
		int gatesize = fanout.size();
		for (int j=0; j<gatesize; j++)
		{
			if ((LookUpTable(fanout[j])!=X)&&(fanout[j]->GetOutput()->GetValue()==X))
			{
				fanout[j]->GetOutput()->SetValue(LookUpTable(fanout[j]));
				wires.push_back(fanout[j]->GetOutput());
			}
			else if ((LookUpTable(fanout[j])!=X)&&(fanout[j]->GetOutput()->GetValue()!=LookUpTable(fanout[j])))
			{
				return false;
			}
		}
		wires.erase(wires.begin());
	}
	return true;
}	


//G: the gate that needs to be objective
//W: wires that contains D
void Objective(Gate* G)
{
	if (G->GetGateType()==AND)
	{
		for (unsigned i=0; i<G->GetInputs().size(); i++)
		{
			if (G->GetInputs()[i]->GetValue()==X)
			{
				G->GetInputs()[i]->SetValue(ONE);
			}
		}
	}
	else if (G->GetGateType()==OR)
	{	
		for (unsigned i=0; i<G->GetInputs().size(); i++)
		{
			if (G->GetInputs()[i]->GetValue()==X)
			{
				G->GetInputs()[i]->SetValue(ZERO);
			}
		}
	}
	else if (G->GetGateType()==NAND)
	{
		for (unsigned i=0; i<G->GetInputs().size(); i++)
		{
			if (G->GetInputs()[i]->GetValue()==X)
			{
				G->GetInputs()[i]->SetValue(ONE);
			}
		}
	}
	else if (G->GetGateType()==NOR)
	{
		for (unsigned i=0; i<G->GetInputs().size(); i++)
		{
			if (G->GetInputs()[i]->GetValue()==X)
			{
				G->GetInputs()[i]->SetValue(ONE);
			}
		}
	}
}


Wire* Backtrace(Gate* G)
{
	Wire* BTResult=NULL;
	for (unsigned i=0; i<G->GetInputs().size(); i++)
	{
		(G->GetInputs())[i]->SetValue(BTLookUpTable(G));
		if ((G->GetInputs())[i]->GetWireType()==INPUT)
		{
			if ((G->GetInputs())[i]->GetBTVisited()==false)
			{	
				(G->GetInputs())[i]->SetValue(BTLookUpTable(G));
				(G->GetInputs())[i]->SetBTVisited(true);
				BTResult = (G->GetInputs())[i];
				break;
			}
		}
		else
		{	
			(G->GetInputs())[i]->SetValue(BTLookUpTable(G));
			BTResult = Backtrace((G->GetInputs())[i]->GetFanIn());
			if (BTResult!=NULL) break;
		}
	}
	return BTResult;
}


//Giving output of Gate G,
//Set the input of Gate G
//This is used only in backtrace
DType BTLookUpTable(Gate* G)
{
	DType OutputValue = G->GetOutput()->GetValue();	
	DType result = X;
	if (G->GetGateType()==NOT)
	{
		if (OutputValue==ONE)
		{result = ZERO;}
		else if (OutputValue==ZERO)
		{result = ONE;}
	}
	else if (G->GetGateType()==AND)
	{
		if (OutputValue==ONE)
		{result = ONE;}
		else if (OutputValue==ZERO)
		{result = ZERO;}
	}
	else if (G->GetGateType()==NAND)
	{
		if (OutputValue==ONE)
		{result = ZERO;}
		else if (OutputValue==ZERO)
		{result = ONE;}
	}
	else if (G->GetGateType()==OR)
	{
		if (OutputValue==ONE)
		{result = ONE;}
		else if (OutputValue==ZERO)
		{result = ZERO;}
	}
	else if (G->GetGateType()==NOR)
	{
		if (OutputValue==ONE)
		{result = ZERO;}
		else if (OutputValue==ZERO)
		{result = ONE;}
	}
	return result;
}

//giving input of the gate G,
//get output of gate G.
DType LookUpTable(Gate* G)
{
	if (G->GetGateType()==NOT)
	{
		DType InputValue = (G->GetInputs())[0]->GetValue();
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
	else if (G->GetGateType()==AND)
	{
		vector<DType> InputValues;
		for (int i=0; i<G->GetInputSize(); i++)
		{
			if((G->GetInputs())[i]->GetValue()==ZERO)
			{return ZERO;}
			else if ((G->GetInputs())[i]->GetValue()==X)
			{return X;}
			else
			{
				InputValues.push_back((G->GetInputs())[i]->GetValue());
			}
		}	
		if (InputValues.size()==2)
		{	
			// input = 11, output = 1;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE))
			{return ONE;}
			//input = 1D, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D))
			{return D;}
			//input = D1, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE))
			{return D;}
			//input 1D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT))
			{return DNOT;}
			//input D'1, output = D'
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE))
			{return DNOT;}
			//input DD, output D
			else if ((InputValues[0]==D)&&(InputValues[1]==D))
			{return D;}
			//input DD', output 0
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT))
			{return ZERO;}
			//input D'D', output D'
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT))
			{return DNOT;}
			//input D'D, output 0
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT))
			{return ZERO;}
		}
		if (InputValues.size()==3)
		{
			// input = 111, output = 1;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return ONE;}
			// input = 11D, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D))
			{return D;}
			// input = 1D1, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE))
			{return D;}
			// input = D11, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return D;}
			// input = 1DD, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return D;}
			// input = D1D, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D))
			{return D;}
			// input = DD1, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE))
			{return D;}
			// input = DDD, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return D;}
			// input = 11D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = 1D'1, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE))
			{return DNOT;}
			// input = D'11, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return DNOT;}
			// input = 1D'D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = D'1D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = D'D'1, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE))
			{return DNOT;}
			// input = D'D'D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = D'1D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==D))
			{return ZERO;}
			// input = D'D1, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ONE))
			{return ZERO;}
			// input = DD'1, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE))
			{return ZERO;}
			// input = D1D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT))
			{return ZERO;}
			// input = 1DD', output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==DNOT))
			{return ZERO;}
			// input = 1D'D, output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==D))
			{return ZERO;}
			// input = DDD', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT))
			{return ZERO;}
			// input = DD'D, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D))
			{return ZERO;}
			// input = D'DD, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return ZERO;}
			// input = D'D'D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE))
			{return ZERO;}
			// input = DD'D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return ZERO;}
			// input = D'DD', output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT))
			{return ZERO;}			
			
		}
		if (InputValues.size()==4)
		{
			// input = 1111, output = 1;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = 111D, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return D;}
			// input = 11D1, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return D;}
			// input = 1D11, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return D;}
			// input = D111, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return D;}
			// input = 111D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = 11D'1, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 1D'11, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = D'111, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 11DD, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = 1D1D, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return D;}
			// input = D11D, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return D;}
			// input = D1D1, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return D;}
			// input = DD11, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return D;}
			// input = 1DD1, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return D;}
			// input = 11D'D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = 1D'1D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'11D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'1D'1, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = D'D'11, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 1D'D'1, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 1DDD, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = D1DD, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = DD1D, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return D;}
			// input = DDD1, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return D;}
			// input = 1D'D'D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D'1D'D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'D'1D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'D'D'1, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = DDDD, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = D'D'D'D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = 11DD', output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = 1D1D', output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D11D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = 11D'D, output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = 1D'1D, output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'11D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return ZERO;}
			// input = DD'11, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = D1D'1, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = 1DD'1, output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = D'D11, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = D'1D1, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = 1D'D1, output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = DDD'1, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = DD1D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D1DD', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = 1DDD', output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = DD'D1, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = D'DD1, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = D'D1D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'1DD, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ZERO;}
			// input = 1D'DD, output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D1D'D, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = DD'1D, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return ZERO;}
			// input = 1DD'D, output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'D'D1, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = D'D'1D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'1D'D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = 1D'D'D, output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'DD'1, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = DD'D'1, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = DD'1D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D1D'D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = 1DD'D', output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D'1DD', output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D'D1D', output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = 1D'DD', output = 0;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = DDD'D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = DD'D'D, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'D'DD, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'DD'D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = DD'DD', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D'DDD', output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D'DDD, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ZERO;}
			// input = DDD'D, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = DD'DD, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ZERO;}
			// input = DDDD', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D'D'D'D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'D'DD', output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D'DD'D', output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = DD'D'D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ZERO;}
		}
	}
	else if (G->GetGateType()==OR)
	{
		vector<DType> InputValues;
		for (int i=0; i<G->GetInputSize(); i++)
		{
			if((G->GetInputs())[i]->GetValue()==ONE)
			{return ONE;}
			else if ((G->GetInputs())[i]->GetValue()==X)
			{return X;}
			else
			{
				InputValues.push_back((G->GetInputs())[i]->GetValue());
			}
		}	
		if (InputValues.size()==2)
		{	
			// input = 00, output = 0;
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO))
			{return ZERO;}
			//input = 0D, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D))
			{return D;}
			//input = D0, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO))
			{return D;}
			//input 0D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT))
			{return DNOT;}
			//input D'0, output = D'
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO))
			{return DNOT;}
			//input DD, output D
			else if ((InputValues[0]==D)&&(InputValues[1]==D))
			{return D;}
			//input DD', output 1
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT))
			{return ONE;}
			//input D'D', output D'
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT))
			{return DNOT;}
			//input D'D, output 1
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT))
			{return ONE;}
		}
		if (InputValues.size()==3)
		{
			// input = OOO, output = 0;
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return ZERO;}
			// input = 00D, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D))
			{return D;}
			// input = 0D0, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO))
			{return D;}
			// input = D00, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return D;}
			// input = 0DD, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return D;}
			// input = D0D, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D))
			{return D;}
			// input = DD0, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO))
			{return D;}
			// input = DDD, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return D;}
			// input = 00D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = 0D'0, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO))
			{return DNOT;}
			// input = D'00, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return DNOT;}
			// input = 0D'D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = D'0D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = D'D'0, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO))
			{return DNOT;}
			// input = D'D'D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = D'0D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==D))
			{return ONE;}
			// input = D'D0, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ZERO))
			{return ONE;}
			// input = DD'0, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO))
			{return ONE;}
			// input = D0D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT))
			{return ONE;}
			// input = 0DD', output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==DNOT))
			{return ONE;}
			// input = 0D'D, output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==D))
			{return ONE;}
			// input = DDD', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT))
			{return ONE;}
			// input = DD'D, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D))
			{return ONE;}
			// input = D'DD, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return ONE;}
			// input = D'D'D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D))
			{return ONE;}
			// input = DD'D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return ONE;}
			// input = D'DD', output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT))
			{return ONE;}			
			
		}
		if (InputValues.size()==4)
		{
			// input = 0000, output = 0;
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = 000D, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return D;}
			// input = 00D0, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 0D00, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return D;}
			// input = D000, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 000D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = 00D'0, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 0D'00, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = D'000, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 00DD, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = 0D0D, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return D;}
			// input = D00D, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return D;}
			// input = D0D0, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return D;}
			// input = DD00, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 0DD0, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 00D'D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = 0D'0D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'00D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'0D'0, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = D'D'00, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 0D'D'0, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 0DDD, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = D0DD, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = DD0D, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return D;}
			// input = DDD0, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 0D'D'D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D'0D'D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'D'0D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'D'D'0, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = DDDD, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = D'D'D'D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = 00DD', output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = 0D0D', output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D00D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = 00D'D, output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = 0D'0D, output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'00D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return ONE;}
			// input = DD'00, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = D0D'0, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = 0DD'0, output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = D'D00, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = D'0D0, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = 0D'D0, output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = DDD'0; output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = DD0D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D0DD', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = 0DDD', output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = DD'D0, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = D'DD0, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = D'D0D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'0DD, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ONE;}
			// input = 0D'DD, output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ONE;}
			// input = D0D'D, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = DD'0D, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return ONE;}
			// input = 0DD'D, output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'D'D0, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = D'D'0D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'0D'D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = 0D'D'D, output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'DD'0, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = DD'D'0, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = DD'0D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D0D'D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = 0DD'D', output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D'0DD', output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D'D0D', output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = 0D'DD', output = 1;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = DDD'D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = DD'D'D, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'D'DD, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'DD'D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = DD'DD', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D'DDD', output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D'DDD, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ONE;}
			// input = DDD'D, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = DD'DD, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ONE;}
			// input = DDDD', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D'D'D'D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'D'DD', output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D'DD'D', output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = DD'D'D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ONE;}
		}
	}
	else if (G->GetGateType()==NAND)
	{
		vector<DType> InputValues;
		for (int i=0; i<G->GetInputSize(); i++)
		{
			if((G->GetInputs())[i]->GetValue()==ZERO)
			{return ONE;}
			else if ((G->GetInputs())[i]->GetValue()==X)
			{return X;}
			else
			{
				InputValues.push_back((G->GetInputs())[i]->GetValue());
			}
		}	
		if (InputValues.size()==2)
		{	
			// input = 11, output = 0;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE))
			{return ZERO;}
			//input = 1D, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D))
			{return DNOT;}
			//input = D1, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE))
			{return DNOT;}
			//input 1D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D))
			{return D;}
			//input D'1, output = D
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE))
			{return D;}
			//input DD, output = D'
			else if ((InputValues[0]==D)&&(InputValues[1]==D))
			{return DNOT;}
			//input DD', output ONE
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT))
			{return ONE;}
			//input D'D', output D
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT))
			{return D;}
			//input D'D, output 1
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT))
			{return ONE;}
		}
		if (InputValues.size()==3)
		{
			// input = 111, output = 0;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return ZERO;}
			// input = 11D, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D))
			{return DNOT;}
			// input = 1D1, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE))
			{return DNOT;}
			// input = D11, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return DNOT;}
			// input = 1DD, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return DNOT;}
			// input = D1D, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D))
			{return DNOT;}
			// input = DD1, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE))
			{return DNOT;}
			// input = DDD, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return DNOT;}
			// input = 11D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT))
			{return D;}
			// input = 1D'1, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE))
			{return D;}
			// input = D'11, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return D;}
			// input = 1D'D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return D;}
			// input = D'1D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT))
			{return D;}
			// input = D'D'1, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE))
			{return D;}
			// input = D'D'D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return D;}
			// input = D'1D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==D))
			{return ONE;}
			// input = D'D1, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ONE))
			{return ONE;}
			// input = DD'1, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE))
			{return ONE;}
			// input = D1D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT))
			{return ONE;}
			// input = 1DD', output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==DNOT))
			{return ONE;}
			// input = 1D'D, output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==D))
			{return ONE;}
			// input = DDD', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT))
			{return ONE;}
			// input = DD'D, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D))
			{return ONE;}
			// input = D'DD, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return ONE;}
			// input = D'D'D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE))
			{return ONE;}
			// input = DD'D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return ONE;}
			// input = D'DD', output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT))
			{return ONE;}			
			
		}
		if (InputValues.size()==4)
		{
			// input = 1111, output = 0;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = 111D, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return DNOT;}
			// input = 11D1, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 1D11, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = D111, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 111D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return D;}
			// input = 11D'1, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return D;}
			// input = 1D'11, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return D;}
			// input = D'111, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return D;}
			// input = 11DD, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = 1D1D, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D11D, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D1D1, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = DD11, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 1DD1, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 11D'D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return D;}
			// input = 1D'1D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'11D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'1D'1, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return D;}
			// input = D'D'11, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return D;}
			// input = 1D'D'1, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return D;}
			// input = 1DDD, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D1DD, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = DD1D, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return DNOT;}
			// input = DDD1, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 1D'D'D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = D'1D'D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'D'1D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'D'D'1, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return D;}
			// input = DDDD, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D'D'D'D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return D;}
			// input = 11DD', output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = 1D1D', output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D11D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = 11D'D, output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = 1D'1D, output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'11D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return ONE;}
			// input = DD'11, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = D1D'1, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = 1DD'1, output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = D'D11, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = D'1D1, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = 1D'D1, output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = DDD'1, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = DD1D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D1DD', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = 1DDD', output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = DD'D1, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = D'DD1, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = D'D1D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'1DD, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ONE;}
			// input = 1D'DD, output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ONE;}
			// input = D1D'D, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = DD'1D, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return ONE;}
			// input = 1DD'D, output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'D'D1, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = D'D'1D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'1D'D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = 1D'D'D, output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'DD'1, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = DD'D'1, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = DD'1D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D1D'D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = 1DD'D', output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D'1DD', output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D'D1D', output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = 1D'DD', output = 1;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = DDD'D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = DD'D'D, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'D'DD, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'DD'D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = DD'DD', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D'DDD', output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D'DDD, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ONE;}
			// input = DDD'D, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = DD'DD, output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ONE;}
			// input = DDDD', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D'D'D'D, output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ONE;}
			// input = D'D'DD', output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = D'DD'D', output = 1;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ONE;}
			// input = DD'D'D', output = 1;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ONE;}
		}
	}
	else if (G->GetGateType()==NOR)
	{
		vector<DType> InputValues;
		for (int i=0; i<G->GetInputSize(); i++)
		{
			if((G->GetInputs())[i]->GetValue()==ONE)
			{return ZERO;}
			else if ((G->GetInputs())[i]->GetValue()==X)
			{return X;}
			else
			{
				InputValues.push_back((G->GetInputs())[i]->GetValue());
			}
		}	
		if (InputValues.size()==2)
		{	
			// input = 00, output = 1;
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO))
			{return ONE;}
			//input = 0D, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D))
			{return DNOT;}
			//input = D0, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO))
			{return DNOT;}
			//input 0D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT))
			{return D;}
			//input D'0, output = D
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO))
			{return D;}
			//input DD, output D'
			else if ((InputValues[0]==D)&&(InputValues[1]==D))
			{return DNOT;}
			//input DD', output 0
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT))
			{return ZERO;}
			//input D'D', output D
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT))
			{return D;}
			//input D'D, output 0
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT))
			{return ZERO;}
		}
		if (InputValues.size()==3)
		{
			// input = OOO, output = 1;
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return ONE;}
			// input = 00D, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D))
			{return DNOT;}
			// input = 0D0, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO))
			{return DNOT;}
			// input = D00, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return DNOT;}
			// input = 0DD, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return DNOT;}
			// input = D0D, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D))
			{return DNOT;}
			// input = DD0, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO))
			{return DNOT;}
			// input = DDD, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return DNOT;}
			// input = 00D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT))
			{return D;}
			// input = 0D'0, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO))
			{return D;}
			// input = D'00, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return D;}
			// input = 0D'D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return D;}
			// input = D'0D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT))
			{return D;}
			// input = D'D'0, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO))
			{return D;}
			// input = D'D'D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return D;}
			// input = D'0D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==D))
			{return ZERO;}
			// input = D'D0, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ZERO))
			{return ZERO;}
			// input = DD'0, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO))
			{return ZERO;}
			// input = D0D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT))
			{return ZERO;}
			// input = 0DD', output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==DNOT))
			{return ZERO;}
			// input = 0D'D, output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==D))
			{return ZERO;}
			// input = DDD', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT))
			{return ZERO;}
			// input = DD'D, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D))
			{return ZERO;}
			// input = D'DD, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return ZERO;}
			// input = D'D'D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D))
			{return ZERO;}
			// input = DD'D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return ZERO;}
			// input = D'DD', output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT))
			{return ZERO;}			
			
		}
		if (InputValues.size()==4)
		{
			// input = 0000, output = 1;
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = 000D, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return DNOT;}
			// input = 00D0, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 0D00, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = D000, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 000D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return D;}
			// input = 00D'0, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 0D'00, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return D;}
			// input = D'000, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 00DD, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = 0D0D, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D00D, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D0D0, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = DD00, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 0DD0, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 00D'D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return D;}
			// input = 0D'0D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'00D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'0D'0, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return D;}
			// input = D'D'00, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 0D'D'0, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 0DDD, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D0DD, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = DD0D, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return DNOT;}
			// input = DDD0, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 0D'D'D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = D'0D'D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'D'0D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'D'D'0, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return D;}
			// input = DDDD, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D'D'D'D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return D;}
			// input = 00DD', output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = 0D0D', output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D00D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = 00D'D, output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = 0D'0D, output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'00D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return ZERO;}
			// input = DD'00, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = D0D'0, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = 0DD'0, output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = D'D00, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = D'0D0, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = 0D'D0, output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = DDD'0; output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = DD0D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D0DD', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = 0DDD', output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = DD'D0, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = D'DD0, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = D'D0D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'0DD, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ZERO;}
			// input = 0D'DD, output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D0D'D, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = DD'0D, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return ZERO;}
			// input = 0DD'D, output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'D'D0, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = D'D'0D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'0D'D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = 0D'D'D, output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'DD'0, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = DD'D'0, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = DD'0D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D0D'D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = 0DD'D', output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D'0DD', output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D'D0D', output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = 0D'DD', output = 0;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = DDD'D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = DD'D'D, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'D'DD, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'DD'D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = DD'DD', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D'DDD', output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D'DDD, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ZERO;}
			// input = DDD'D, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = DD'DD, output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return ZERO;}
			// input = DDDD', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D'D'D'D, output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==D))
			{return ZERO;}
			// input = D'D'DD', output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==D)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = D'DD'D', output = 0;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ZERO;}
			// input = DD'D'D', output = 0;
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return ZERO;}
		}
	}
	return X;
}
