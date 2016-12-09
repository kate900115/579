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
void ClearObjFixed();
void ClearDFrontierVisited();

vector<Wire*> CWire;
vector<Gate*> CGate;
vector<Wire*> InputWires;
vector<Wire*> ComputedInputs;
vector<Gate*> DFrontiers;
vector<Wire*> OutputWires;

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
				OutputWires.push_back(NewOutput);
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
		
			Gate* NewDFF = new Gate(dff_name, BUFFER, DffInputs, DffOutput);
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
		ClearObjFixed();
		ClearDFrontierVisited();
		cout<<"@@@  Wire "<<CWire[i]->GetWireName()<<" is the fault site. It's s-a-0 fault."<<endl;

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
		//clear DFrontier List
		while (!DFrontiers.empty())
		{
			DFrontiers.pop_back();
		}

		if(PODEM(PodemWire)==true)
		{	
			TestNumber++;
			cout<<endl;
			cout<<"@@@ Wire "<<CWire[i]->GetWireName()<<"/0 has test vector"<<endl;
			cout<<endl;
		}
		//Do PODEM
		else
		{
			cout<<endl;
			cout<<"fail!"<<endl;
			cout<<endl;
			DFrontiers.pop_back();					
			ClearObjFixed();
			cout<<"D SIZE = "<<DFrontiers.size()<<endl;
			if (!DFrontiers.empty())
			{
				for (unsigned m=0; m<DFrontiers.back()->GetInputs().size(); m++)
				{
					if ((DFrontiers.back()->GetInputs()[m]->GetValue()==D)||(DFrontiers.back()->GetInputs()[m]->GetValue()==DNOT))
					{
						PodemWire = DFrontiers.back()->GetInputs()[m];
						break;
					}
				}
			}
			bool success=false;
			
			while (!DFrontiers.empty())
			{
				
				if(PODEM(PodemWire)==true)
				{	
					TestNumber++;
					cout<<endl;
					cout<<"@@ Wire "<<CWire[i]->GetWireName()<<"/0 has test vector"<<endl;
					cout<<endl;
					success = true;
					break;
				}
				else
				{
					cout<<endl;
					cout<<"fail!"<<endl;
					cout<<endl;
					DFrontiers.pop_back();
					ClearObjFixed();
					cout<<"D SIZE = "<<DFrontiers.size()<<endl;
					if (!DFrontiers.empty())
					{
						for (unsigned m=0; m<DFrontiers.back()->GetInputs().size(); m++)
						{
							if ((DFrontiers.back()->GetInputs()[m]->GetValue()==D)||(DFrontiers.back()->GetInputs()[m]->GetValue()==DNOT))
							{
								PodemWire = DFrontiers.back()->GetInputs()[m];
								break;
							}
						}	
					}
					else
					{
						break;
					}
				}
			}
			if (!success)
			{
				cout<<endl;
				cout<<"@@ Wire "<<CWire[i]->GetWireName()<<"/0 has no test vector"<<endl;
				cout<<endl;
			}
			
		}


		//---------------------------------------------------------------------------------------------------------------
				//intialize and set s-a-1 fault
		Initialize();
		ClearObjFixed();
		ClearDFrontierVisited();
		cout<<endl<<"@@  Wire "<<CWire[i]->GetWireName()<<" is the fault site. It's s-a-1 fault."<<endl;

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
		//clear DFrontier List
		while (!DFrontiers.empty())
		{
			DFrontiers.pop_back();
		}

		if(PODEM(PodemWire)==true)
		{	
			TestNumber++;
			cout<<endl;
			cout<<endl<<"@@ Wire "<<CWire[i]->GetWireName()<<"/1 has test vector"<<endl;
			cout<<endl;
		}
		//Do PODEM
		else
		{
			cout<<endl;
			cout<<"fail!"<<endl;
			cout<<endl;
			DFrontiers.pop_back();					
			ClearObjFixed();
			
			cout<<"D SIZE = "<<DFrontiers.size()<<endl;
			if (!DFrontiers.empty())
			{
				for (unsigned m=0; m<DFrontiers.back()->GetInputs().size(); m++)
				{
					if ((DFrontiers.back()->GetInputs()[m]->GetValue()==D)||(DFrontiers.back()->GetInputs()[m]->GetValue()==DNOT))
					{
						PodemWire = DFrontiers.back()->GetInputs()[m];
						break;
					}
				}
			}
			bool success=false;
			
			while (!DFrontiers.empty())
			{
				
				if(PODEM(PodemWire)==true)
				{	
					TestNumber++;
					cout<<endl;
					cout<<"@@ Wire "<<CWire[i]->GetWireName()<<"/1 has test vector"<<endl;
					cout<<endl;
					success = true;
					break;
				}
				else
				{
					cout<<endl;
					cout<<"fail!"<<endl;
					cout<<endl;
					DFrontiers.pop_back();
					ClearObjFixed();
					cout<<"D SIZE = "<<DFrontiers.size()<<endl;
					if (!DFrontiers.empty())
					{
						for (unsigned m=0; m<DFrontiers.back()->GetInputs().size(); m++)
						{
							if ((DFrontiers.back()->GetInputs()[m]->GetValue()==D)||(DFrontiers.back()->GetInputs()[m]->GetValue()==DNOT))
							{
								PodemWire = DFrontiers.back()->GetInputs()[m];
								break;
							}
						}	
					}
					else
					{
						break;
					}
				}
			}
			if (!success)
			{
				cout<<endl;
				cout<<"@@ Wire "<<CWire[i]->GetWireName()<<"/1 has no test vector"<<endl;
				cout<<endl;
			}
			
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
		//pick up a gate from D-frontier
		//to do objective()

		vector <Gate*> FrontierGates = CurrentWire->GetFanOut();
		int F_size = FrontierGates.size();
		/*--------------------for test-------------------*/
		cout<<"frontier gates are: ";
		for(unsigned i=0; i<FrontierGates.size(); i++)
		{
			cout<<FrontierGates[i]->GetGateName()<<",";
		}
		cout<<endl;
		/*--------------------for test-------------------*/


		/*--------------------for test-------------------*/
		cout<<endl<<"Gate information:"<<endl;
		int GateSize=CGate.size();
		for (int i=0; i<GateSize; i++)
		{
			CGate[i]->PrintGate();
			for (int j=0; j<CGate[i]->GetInputSize(); j++)
			{
				cout<<"input"<<j<<"="<<(CGate[i]->GetInputs())[j]->GetWireName()<<", ";
			}
			cout<<"output = "<<CGate[i]->GetOutput()->GetWireName()<<", DFrontierVisited: "<<CGate[i]->GetDFrontierVisited()<<endl;
		}

		/*--------------------for test-------------------*/

		vector <Gate*> NotLists;

		//update DFrontier list
		for (int i=0; i<F_size; i++)
		{
			if (FrontierGates[i]->GetGateType()==NOT)
			{	
				NotLists.push_back(FrontierGates[i]);
			}
			else if (FrontierGates[i]->GetGateType()==BUFFER)
			{	
				NotLists.push_back(FrontierGates[i]);
			}
			else if (!FrontierGates[i]->GetDFrontierVisited())
			{
				DFrontiers.push_back(FrontierGates[i]);
				FrontierGates[i]->SetDFrontierVisited(true);
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
				if (NotLists.front()->GetOutput()->GetWireType()!=OUTPUT)
				{
					vector<Gate*> tempgate = NotLists.front()->GetOutput()->GetFanOut();
					NotLists.insert(NotLists.end(), tempgate.begin(), tempgate.end());
					
					//NotLists.push_back(NotLists.front()->GetOutput()->GetFanOut()[0]);
				}
				else
				{
					CurrentWire = NotLists.front()->GetOutput();
					return true;
				}
			}
			else if (NotLists.front()->GetGateType()==BUFFER)
			{
				if (NotLists.front()->GetInputs()[0]->GetValue()==D)
				{
					NotLists.front()->GetOutput()->SetValue(D);
				}
				else if(NotLists.front()->GetInputs()[0]->GetValue()==DNOT)
				{
					NotLists.front()->GetOutput()->SetValue(DNOT);
				}
				if (NotLists.front()->GetOutput()->GetWireType()!=OUTPUT)
				{
					vector<Gate*> tempgate = NotLists.front()->GetOutput()->GetFanOut();
					NotLists.insert(NotLists.end(), tempgate.begin(), tempgate.end());
					
				}
				else
				{
					CurrentWire = NotLists.front()->GetOutput();
					return true;
				}
			}
			else if (!NotLists.front()->GetDFrontierVisited())
			{
				DFrontiers.push_back(NotLists.front());
				NotLists.front()->SetDFrontierVisited(true);
				
			}	
			NotLists.erase(NotLists.begin());
		}

		//If No DFrontier, untestable
		//return false

		if (DFrontiers.size()==0)
		{
			cout<<"DFROTIER SIZE ="<<DFrontiers.size()<<endl;
			cout<<"No D-Frontier!!!"<<endl;
			return false;
		}

		/*--------------------for test--------------------*/
		cout<<"DFrontiers are: ";
		for (unsigned i=0; i<DFrontiers.size(); i++)
		{
			cout<<DFrontiers[i]->GetGateName()<<",";
		}	
		cout<<endl;
		/*--------------------for test--------------------*/
		
		//update current wire and frontier gate
		FrontierGate = DFrontiers.back();
		for (unsigned m=0; m<FrontierGate->GetInputs().size(); m++)
		{
			if ((FrontierGate->GetInputs()[m]->GetValue()==D)||(FrontierGate->GetInputs()[m]->GetValue()==DNOT))
			{
				CurrentWire = FrontierGate->GetInputs()[m];
				break;
			}
		}
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
		cout<<endl;
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
			

			CurrentWire = FrontierGate->GetOutput();

			while (CurrentWire->GetWireType()!=OUTPUT)
			{
				if ((CurrentWire->GetFanOut().size()==1)&&(CurrentWire->GetFanOut()[0]->GetGateType()==NOT))
				{
					if (CurrentWire->GetValue()==D)
					{
						CurrentWire = CurrentWire->GetFanOut()[0]->GetOutput();
						CurrentWire->SetValue(DNOT);
					}
					else
					{
						CurrentWire = CurrentWire->GetFanOut()[0]->GetOutput();
						CurrentWire->SetValue(D);
					}
				}
				else if ((CurrentWire->GetFanOut().size()==1)&&(CurrentWire->GetFanOut()[0]->GetGateType()==BUFFER))
				{
					if (CurrentWire->GetValue()==D)
					{
						CurrentWire = CurrentWire->GetFanOut()[0]->GetOutput();
						CurrentWire->SetValue(D);
					}
					else
					{
						CurrentWire = CurrentWire->GetFanOut()[0]->GetOutput();
						CurrentWire->SetValue(DNOT);
					}
				}
				else
				{
					break;
				}
			}

			
			FrontierGates = CurrentWire->GetFanOut();

			vector<Gate*> GateTest = FrontierGates;
			
			// if all the DFrontier are visited
			// we need to exit
			bool NewDFrontier = false;
			while(!GateTest.empty())
			{	
				cout<<"aaaaaaaaaaaaaaaaaaaaaaa"<<endl;
				if ((GateTest.front()->GetGateType()!=NOT)&&(GateTest.front()->GetGateType()!=BUFFER))
				{
					if (!GateTest.front()->GetDFrontierVisited())
					{
						NewDFrontier = true;
						break;
					}
				}
				else
				{
					if (GateTest.front()->GetOutput()->GetWireType()!=OUTPUT)
					{
						vector<Gate*> ToBeAdd = GateTest.front()->GetOutput()->GetFanOut();
						GateTest.insert(GateTest.end(), ToBeAdd.begin(), ToBeAdd.end());
					}
				}
				GateTest.erase(GateTest.begin());
			}

			if ((NewDFrontier==false)&&(CurrentWire->GetWireType()!=OUTPUT))
			{
				return false;
			}

			/*--------------------for test-------------------*/
			cout<<"frontier gates are: ";
			for(unsigned i=0; i<FrontierGates.size(); i++)
			{
				cout<<FrontierGates[i]->GetGateName()<<",";
			}
			cout<<endl;
			/*--------------------for test-------------------*/

			/*--------------------for test--------------------*/
			cout<<"DFrontiers are: ";
			for (unsigned i=0; i<DFrontiers.size(); i++)
			{
				cout<<DFrontiers[i]->GetGateName()<<",";
			}	
			cout<<endl;
			/*--------------------for test--------------------*/

			DFrontiers.pop_back();

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
		cout<<"D11 SIZE = "<<DFrontiers.size()<<endl;
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

void ClearObjFixed()
{
	int size = CWire.size();
	for (int i=0; i<size; i++)
	{
		CWire[i]->SetObjFixed(false);
	}
}

void ClearDFrontierVisited()
{
	int size = CGate.size();
	for (int i=0; i<size; i++)
	{
		CGate[i]->SetDFrontierVisited(false);
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

		else if (gates.front()->GetGateType()==BUFFER)
		{
			if (gates.front()->GetOutput()->GetValue()==ONE)
			{
				(gates.front()->GetInputs())[0]->SetValue(ONE);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if (gates.front()->GetInputs()[0]->GetWireType()!=INPUT)
				{
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}

			}
			else if (gates.front()->GetOutput()->GetValue()==ZERO)
			{
				(gates.front()->GetInputs())[0]->SetValue(ZERO);
				//gates.front()->SetVisited(true);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if (gates.front()->GetInputs()[0]->GetWireType()!=INPUT)
				{	
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==D)
			{	
				(gates.front()->GetInputs())[0]->SetValue(ONE);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if (gates.front()->GetInputs()[0]->GetWireType()!=INPUT)
				{
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==DNOT)
			{	
				(gates.front()->GetInputs())[0]->SetValue(ZERO);
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
		else if (gates.front()->GetGateType() == BUFFER)
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

			if ( (LookUpTable(fanout[j])!=X)
				&&(  ! (  (fanout[j]->GetOutput()->GetValue()==LookUpTable(fanout[j]))
				||        (fanout[j]->GetOutput()->GetValue()==DLookUpTable(fanout[j])) ))
				&&((fanout[j]->GetOutput()->GetFixed()==true)||(fanout[j]->GetOutput()->GetObjFixed()==true)))
			{
				cout<<fanout[j]->GetGateName()<<" return false"<<endl;
				return false;
			}
			else if ((fanout[j]->GetOutput()->GetFixed()==false)&&(fanout[j]->GetOutput()->GetObjFixed()==false))
			{
				fanout[j]->GetOutput()->SetValue(LookUpTable(fanout[j]));
				wires.push_back(fanout[j]->GetOutput());
			}
		}
		wires.erase(wires.begin());
	}
	bool success=false;
	for (unsigned i = 0; i<OutputWires.size(); i++)
	{
		if((OutputWires[i]->GetValue()== DNOT)||(OutputWires[i]->GetValue()== D)||(OutputWires[i]->GetValue()== X))
		{
			cout<<"return true"<<endl;
			success = true;
		}
	}
	return success;
}	


	
//G: the gate that needs to be objective
//W: wires that contains D
void Objective(Gate* G)
{
	if (G->GetGateType()==AND)
	{
		for (unsigned i=0; i<G->GetInputs().size(); i++)
		{
			//if (G->GetInputs()[i]->GetValue()==X)
			if ((G->GetInputs()[i]->GetValue()!=D)&&(G->GetInputs()[i]->GetValue()!=DNOT))
			{
				G->GetInputs()[i]->SetValue(ONE);
				G->GetInputs()[i]->SetObjFixed(true);
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
			if ((G->GetInputs()[i]->GetValue()!=D)&&(G->GetInputs()[i]->GetValue()!=DNOT))
			{
				G->GetInputs()[i]->SetValue(ZERO);
				G->GetInputs()[i]->SetObjFixed(true);
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
			if ((G->GetInputs()[i]->GetValue()!=D)&&(G->GetInputs()[i]->GetValue()!=DNOT))
			{
				G->GetInputs()[i]->SetValue(ONE);
				G->GetInputs()[i]->SetObjFixed(true);
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
			if ((G->GetInputs()[i]->GetValue()!=D)&&(G->GetInputs()[i]->GetValue()!=DNOT))
			{
				G->GetInputs()[i]->SetValue(ZERO);
				G->GetInputs()[i]->SetObjFixed(true);
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
			if (((G->GetInputs())[i]->GetBTVisited()==false)
			  &&((G->GetInputs())[i]->GetFixed()==false)&&((G->GetInputs())[i]->GetObjFixed()==false))
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
