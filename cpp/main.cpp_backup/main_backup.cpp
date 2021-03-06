#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include "wire.h"
#include "truthtable.h"
#include <string>

using namespace std;

bool PODEM(Wire* W);
void Initialize();
void ImplyBackward(Gate* G);
bool InputImplyForward();
Gate* ImplyForward(vector<Gate*> Gs);
Wire* Backtrace(Gate* G);
void Objective(Gate* G);
DType LookUpTable(Gate* G);
DType DLookUpTable(Gate* G);

vector<Wire*> CWire;
vector<Gate*> CGate;
vector<Wire*> InputWires;
vector<Wire*> ComputedInputs;
vector<Gate*> DFrontiers;

Gate* FrontierGate;

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
		//intialize and set s-a-1 fault
		Initialize();
		cout<<"@@  Wire "<<CWire[i]->GetWireName()<<" is the fault site. It's s-a-0 fault."<<endl;

		CWire[i]->SetStuck(true,D);
		CWire[i]->SetFixed(true);
		if (CWire[i]->GetWireType()==INPUT)
		{
			CWire[i]->SetBTVisited(true);
		}

		//Activate fault
		//Implication to the back
		
	
		
		if (CWire[i]->GetWireType()!=INPUT)
		{
			Gate* DBack = CWire[i]->GetFanIn();
			ImplyBackward(DBack);
		}
		
		//find D-frontier
		//imply forward if there are NOT gates
		//and then update the frontier wire
		vector<Gate*> DFront = CWire[i]->GetFanOut();
		Wire* PodemWire = CWire[i];
		Gate* InitialFrontier = ImplyForward(DFront);
		if (InitialFrontier != NULL)
		{
			PodemWire = InitialFrontier->GetInputs()[0];
		}	

		//for test
		//PodemWire->PrintWire();
		/*--------------------for test--------------------*/
		//print all the Wire read from input file
		int WireSize = CWire.size();
		for (int m=0; m<WireSize; m++)
		{
			CWire[m]->PrintWire();
		}
		/*--------------------for test--------------------*/

		////////////////////////////////////////////////////////////////////////
		//push the DFrontier in
		while (!DFrontiers.empty())
		{
			DFrontiers.pop_back();
		}
		DFrontiers = PodemWire->GetFanOut();
	//	DFrontiers.push_back(PodemWire->GetFanout[0]);

	//	cout<<DFrontiers.size()<<endl;
	//	cout<<InitialFrontier->GetGateName()<<endl;
	//	cout<<DFrontiers.front()->GetGateName()<<endl;

		//Do PODEM
		if(PODEM(PodemWire)==true)
		{	
			TestNumber++;
			cout<<endl;
			cout<<"@@ Wire "<<CWire[i]->GetWireName()<<"/0 has test vector"<<endl;
			cout<<endl;
		}
		else
		{
			cout<<endl;
			cout<<"@@ Wire "<<CWire[i]->GetWireName()<<"/0 has no test vector"<<endl;
			cout<<endl;
		}


		/*--------------------for test--------------------*/
		// to test the DFrontier
	/*	int DF_size = DFrontiers.size();
		if (DFrontiers.size()!=0)
		{
			cout<<"DFrontier list is shown below:"<<endl;
			for (int i=0; i<DF_size; i++)
			{
				cout<<DFrontiers[i]->GetGateName();
				cout<<endl;
			}
		}*/



		//---------------------------------------------------------------------------------------------------------------
		//initialize and set s-a-0 fault
		Initialize();
		cout<<"@@  Wire "<<CWire[i]->GetWireName()<<" is the fault site. It's s-a-0 fault."<<endl;

		CWire[i]->SetStuck(true,DNOT);
		CWire[i]->SetFixed(true);
		if (CWire[i]->GetWireType()==INPUT)
		{
			CWire[i]->SetBTVisited(true);
		}

		//Activate fault
		//Implication to the back
		if (CWire[i]->GetWireType()!=INPUT)
		{
			Gate* DBack = CWire[i]->GetFanIn();
			ImplyBackward(DBack);
		}
		
		//find D-frontier
		//imply forward if there are NOT gates
		//and then update the frontier wire
		DFront = CWire[i]->GetFanOut();
		PodemWire = CWire[i];
		InitialFrontier = ImplyForward(DFront);
		if (InitialFrontier != NULL)
		{
			PodemWire = InitialFrontier->GetInputs()[0];
		}	

		//for test
		//PodemWire->PrintWire();
		/*--------------------for test--------------------*/
		//print all the Wire read from input file
		WireSize = CWire.size();
		for (int m=0; m<WireSize; m++)
		{
			CWire[m]->PrintWire();
		}
		/*--------------------for test--------------------*/

		////////////////////////////////////////////////////////////////////////
		//push the DFrontier in
		while (!DFrontiers.empty())
		{
			DFrontiers.pop_back();
		}
		DFrontiers = PodemWire->GetFanOut();

		//Do PODEM
		if(PODEM(PodemWire)==true)
		{	
			TestNumber++;
			cout<<endl;
			cout<<"@@ Wire "<<CWire[i]->GetWireName()<<"/1 has test vector"<<endl;
			cout<<endl;
		}
		else
		{
			cout<<endl;
			cout<<"@@ Wire "<<CWire[i]->GetWireName()<<"/1 has no test vector"<<endl;
			cout<<endl;
		}


		/*--------------------for test--------------------*/
		// to test the DFrontier
	/*	DF_size = DFrontiers.size();
		if (DFrontiers.size()!=0)
		{
			cout<<"DFrontier list is shown below:"<<endl;
			for (int i=0; i<DF_size; i++)
			{
				DFrontiers[i]->PrintGate();
				cout<<endl;
			}
		}*/

	}

	return 0;
}



//functions

bool PODEM(Wire* W)
{
	//if Stuck at fault is at primary output
	//we don't need to generate test vector
	Wire* CurrentWire = W;	

	/*--------------------for test--------------------*/
	//print all the Gate read from input file 
	cout<<endl;
	cout<<"-----------------------------PODOM--------------------------------"<<endl;



	int WireSize = CWire.size();
	cout<<"CurrentWire = ";
	CurrentWire->PrintWire();
	cout<<endl;
	/*--------------------for test--------------------*/
		
	if ((CurrentWire->GetWireType()==OUTPUT)&&(CurrentWire->GetValue()!=X)&&(CurrentWire->GetValue()!=ZERO)&&(CurrentWire->GetValue()!=ONE)) 
		return true;
	else
	{
		

	
	//If No DFrontier, untestable
	//return false

/*
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
*/

	
	//pick up a gate from D-frontier
	//to do objective()

	vector <Gate*> FrontierGates = CurrentWire->GetFanOut();
	int F_size = FrontierGates.size();

	vector <Gate*> NotLists;

	for (int i=0; i<F_size; i++)
	{
		if (FrontierGates[i]->GetGateType()==NOT)
		{
			NotLists.push_back(FrontierGates[i]);
		}
		else
		{
			bool update_flag = true;
			int D_size = DFrontiers.size();
			for (int i=0; i<D_size; i++)
			{
				if (DFrontiers[i]==FrontierGates.front())
				{
					update_flag = false;
					break;
				}
			}
			if (update_flag)
			{
				DFrontiers.push_back(FrontierGates.front());
			}
		}
	}

	while (!NotLists.empty())
	{
		if (NotLists.front()->GetGateType()==NOT)
		{
			if (NotLists.front()->GetInputs()[0]->GetValue()==D)
			{
				NotLists.front()->GetOutput()->SetValue(DNOT);
			}
			else if(NotLists.front()->GetInputs()[0]->GetValue()==DNOT)
			{
				NotLists.front()->GetOutput()->SetValue(D);
			}
			NotLists.push_back(NotLists.front()->GetOutput()->GetFanOut()[0]);
		}
		else
		{
			bool update_flag = true;
			int D_size = DFrontiers.size();
			for (int i=0; i<D_size; i++)
			{
				if (DFrontiers[i]==FrontierGates.front())
				{
					update_flag = false;
					break;
				}
			}
			if (update_flag)
			{
				DFrontiers.push_back(NotLists.front());
			}
		}	
		NotLists.erase(NotLists.begin());
	}

	

	if (DFrontiers.size()==0)	
	{
		return false;
	}
/*
	while (FrontierGates.size()!=0)
	{
		if (FrontierGates.front()->GetGateType()==NOT)
		{
			if ((FrontierGates.front()->GetInputs())[0]->GetValue()==D) FrontierGates.front()->GetOutput()->SetValue(DNOT);
			else if ((FrontierGate->GetInputs())[0]->GetValue()==DNOT) FrontierGates.front()->GetOutput()->SetValue(D);
			FrontierGates.push_back(FrontierGates.front()->GetOutput()->GetFanOut()[0]);
			//FrontierGate = FrontierGates.front();
			CurrentWire = FrontierGate->GetOutput();
		}
		else
		{
			int D_size = DFrontiers.size();
			bool update_flag = true;
			for (int i=0; i<D_size; i++)
			{
				if (DFrontiers[i]==FrontierGates.front())
				{
					cout<<"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"<<endl;
					update_flag = false;
					break;
				}
			}
			
			if (update_flag)
			{
				DFrontiers.push_back(FrontierGates.front());
			}
		}
		
		FrontierGates.erase(FrontierGates.begin());
	}
	*/
	/*--------------------for test--------------------*/
	cout<<"DFrontiers are: ";
	for (unsigned i=0; i<DFrontiers.size(); i++)
	{
		cout<<DFrontiers[i]->GetGateName()<<",";
	}	
	cout<<endl;
	/*--------------------for test--------------------*/

	FrontierGate = DFrontiers.back();
	//DFrontiers.erase(DFrontiers.begin());

	

	/*--------------------for test--------------------*/
	cout<<"{NEW} Frontier Gate is:";	
	FrontierGate->PrintGate();
	cout<<endl;
	/*--------------------for test--------------------*/

	//objective
	Objective(FrontierGate);
	FrontierGate->GetOutput()->SetValue(LookUpTable(FrontierGate));

	/*--------------------for test--------------------*/
	cout<<"-------after objective---------"<<endl;
	for (int m=0; m<WireSize; m++)
	{
		CWire[m]->PrintWire();
	}
	/*--------------------for test--------------------*/

	//Backtrace
	Wire* BTResult = Backtrace(FrontierGate);
	/*--------------------for test--------------------*/
	cout<<"-------after backtrace---------"<<endl;
	for (int m=0; m<WireSize; m++)
	{
		CWire[m]->PrintWire();
	}
	/*--------------------for test--------------------*/

		
	//if all the Backtrace is done, current wire change 
	//and frontier gate changes
	if(BTResult==NULL)
	{
		//if the frontier gate is not gate
		//we need to propagate it further
		//until the frontier is not a Not gate.
	//	FrontierGates = CurrentWire->GetFanOut();
		CurrentWire = FrontierGate->GetOutput();
		FrontierGates = CurrentWire->GetFanOut();
		DFrontiers.pop_back();
	/*	cout<<FrontierGates.size()<<endl;
		while (FrontierGates.size()!=0)
		{
			if (FrontierGates.front()->GetGateType()==NOT)
			{
				if ((FrontierGates.front()->GetInputs())[0]->GetValue()==D) FrontierGates.front()->GetOutput()->SetValue(DNOT);
				else if ((FrontierGate->GetInputs())[0]->GetValue()==DNOT) FrontierGates.front()->GetOutput()->SetValue(D);
				FrontierGates.push_back(FrontierGates.front()->GetOutput()->GetFanOut()[0]);
				//FrontierGate = FrontierGates.front();
				CurrentWire = FrontierGate->GetOutput();
			}
			else
			{
				DFrontiers.push_back(FrontierGates.front());
			}
			FrontierGates.erase(FrontierGates.begin());
		}
		FrontierGate = DFrontiers.back();
		DFrontiers.pop_back();	*/
	}

	//ImplyForward BTResult to see if there is a contradiction
	if (InputImplyForward())
	{
		/*--------------------for test--------------------*/
		cout<<"-------after imply---------"<<endl;
		for (int m=0; m<WireSize; m++)
		{
			CWire[m]->PrintWire();
		}
		/*--------------------for test--------------------*/
		if (PODEM(CurrentWire)==true) return true;
	}
	if (BTResult==NULL)
	{
		return false;
	}
	
	//implyForward BTResult' to see if there is a contradiction
	if ((BTResult!=NULL)&&(BTResult->GetFixed()==true))
		{return false;}
	if(BTResult->GetValue()==ONE)
		{BTResult->SetValue(ZERO);}
	else if (BTResult->GetValue()==ZERO)
		{BTResult->SetValue(ONE);}
	/*--------------------for test--------------------*/
	cout<<"---fail, we need to backtrack.---"<<endl;
	for (int m=0; m<WireSize; m++)
	{
		CWire[m]->PrintWire();
	}
	/*--------------------for test--------------------*/

	if(InputImplyForward())
	{
		/*--------------------for test--------------------*/
		cout<<"-------after imply---------"<<endl;
		for (int m=0; m<WireSize; m++)
		{
			CWire[m]->PrintWire();
		}
		/*--------------------for test--------------------*/

		if (PODEM(CurrentWire)==true) return true;
	}

	//Imply BTResult= X
	BTResult->SetValue(X);
	BTResult->SetFixed(false);
	BTResult->SetBTVisited(false);
	InputImplyForward();
	return false;
	}
}


//set all the wire to X
void Initialize()
{
	int size = CWire.size();
	for (int i=0; i<size; i++)
	{
		CWire[i]->initialize();
	}
}




void ImplyBackward(Gate* G)
{
	vector<Gate*> gates;
	gates.push_back(G);
	while (gates.size()!=0)
	{
		gates.front()->PrintGate();
		cout<<endl;
		if (gates.front()->GetGateType()==NOT)
		{
			if (gates.front()->GetOutput()->GetValue()==ONE)
			{
				(gates.front()->GetInputs())[0]->SetValue(ZERO);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if (gates.front()->GetInputs()[0]->GetWireType()!=INPUT)
				{
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}

			}
			else if (gates.front()->GetOutput()->GetValue()==ZERO)
			{
				(gates.front()->GetInputs())[0]->SetValue(ONE);
				//gates.front()->SetVisited(true);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if (gates.front()->GetInputs()[0]->GetWireType()!=INPUT)
				{	
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==D)
			{	
				(gates.front()->GetInputs())[0]->SetValue(ZERO);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if (gates.front()->GetInputs()[0]->GetWireType()!=INPUT)
				{
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==DNOT)
			{	
				(gates.front()->GetInputs())[0]->SetValue(ONE);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if (gates.front()->GetInputs()[0]->GetWireType()!=INPUT)
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
					gates.front()->GetInputs()[i]->SetFixed(true);
					if (gates.front()->GetInputs()[i]->GetWireType()!=INPUT)
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
					gates.front()->GetInputs()[i]->SetFixed(true);
					if (gates.front()->GetInputs()[i]->GetWireType()!=INPUT)
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
					gates.front()->GetInputs()[i]->SetFixed(true);
					if (gates.front()->GetInputs()[i]->GetWireType()!=INPUT)
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
					gates.front()->GetInputs()[i]->SetFixed(true);
					if (gates.front()->GetInputs()[i]->GetWireType()!=INPUT)
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
					gates.front()->GetInputs()[i]->SetFixed(true);
					if (gates.front()->GetInputs()[i]->GetWireType()!=INPUT)
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
					gates.front()->GetInputs()[i]->SetFixed(true);
					if (gates.front()->GetInputs()[i]->GetWireType()!=INPUT)
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
					gates.front()->GetInputs()[i]->SetFixed(true);
					if (gates.front()->GetInputs()[i]->GetWireType()!=INPUT)
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
					gates.front()->GetInputs()[i]->SetFixed(true);
					if (gates.front()->GetInputs()[i]->GetWireType()!=INPUT)
					{
						gates.push_back((gates.front()->GetInputs())[i]->GetFanIn());
					}
				}
			}
		}
		gates.erase(gates.begin());
	}
}


Gate* ImplyForward(vector<Gate*> Gs)
{
	vector<Gate*> gates = Gs;
	Gate* NewFrontier = NULL;
	//Implication to the front
	//if there is NOT gate as frontier
	//We can propagate D or D' further
	while (gates.size()!=0)
	{
		if (gates.front()->GetGateType() == NOT)
		{
			gates.front()->GetOutput()->SetValue(LookUpTable(gates.front()));
			gates.front()->GetOutput()->SetFixed(true);
			if (gates.front()->GetOutput()->GetWireType()!=OUTPUT)
			{
				NewFrontier = (gates.front()->GetOutput()->GetFanOut())[0];
				gates.push_back((gates.front()->GetOutput()->GetFanOut())[0]);
			}
		}
		gates.erase(gates.begin());	
	}
	return NewFrontier;
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

			if (fanout[j]->GetOutput()->GetFixed()==false)
			{
				fanout[j]->GetOutput()->SetValue(LookUpTable(fanout[j]));
				wires.push_back(fanout[j]->GetOutput());
			}
			else if ( (LookUpTable(fanout[j])!=X)
				&&(  ! (  (fanout[j]->GetOutput()->GetValue()==LookUpTable(fanout[j]))
				||        (fanout[j]->GetOutput()->GetValue()==DLookUpTable(fanout[j])) ))
				&&(fanout[j]->GetOutput()->GetFixed()==true))
			{
				cout<<fanout[j]->GetGateName()<<" return false"<<endl;
				return false;
			}
		}
		wires.erase(wires.begin());
	}
	cout<<"return true"<<endl;
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
				G->GetInputs()[i]->SetFixed(true);
				if (G->GetInputs()[i]->GetWireType()==INPUT)
				{
					G->GetInputs()[i]->SetBTVisited(true);
				}
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
				G->GetInputs()[i]->SetFixed(true);
				if (G->GetInputs()[i]->GetWireType()==INPUT)
				{
					G->GetInputs()[i]->SetBTVisited(true);
				}
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
				G->GetInputs()[i]->SetFixed(true);
				if (G->GetInputs()[i]->GetWireType()==INPUT)
				{
					G->GetInputs()[i]->SetBTVisited(true);
				}
			}
		}
	}
	else if (G->GetGateType()==NOR)
	{
		for (unsigned i=0; i<G->GetInputs().size(); i++)
		{
			if (G->GetInputs()[i]->GetValue()==X)
			{
				G->GetInputs()[i]->SetValue(ZERO);
				G->GetInputs()[i]->SetFixed(true);
				if (G->GetInputs()[i]->GetWireType()==INPUT)
				{
					G->GetInputs()[i]->SetBTVisited(true);
				}
			}
		}
	}
}


Wire* Backtrace(Gate* G)
{
	Wire* BTResult=NULL;
	for (unsigned i=0; i<G->GetInputs().size(); i++)
	{
		if ((G->GetInputs())[i]->GetWireType()==INPUT)
		{
			if (((G->GetInputs())[i]->GetBTVisited()==false)&&((G->GetInputs())[i]->GetFixed()==false))
			{	
				(G->GetInputs())[i]->SetValue(ZERO);
				(G->GetInputs())[i]->SetBTVisited(true);
				BTResult = (G->GetInputs())[i];
				break;
			}
		}
		else
		{	
			BTResult = Backtrace((G->GetInputs())[i]->GetFanIn());
			if (BTResult!=NULL) break;
		}
	}
	return BTResult;
}
