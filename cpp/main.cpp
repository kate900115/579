#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include "wire.h"
#include "truthtable.h"
#include <string>

using namespace std;

//functions
bool PODEM(Wire* W, int frame);
void Initialize(int frame);
void ImplyBackward(Gate* G);
bool InputImplyForward(int frame);
Wire* ImplyForward(vector<Gate*> Gs, int frame);
Wire* Backtrace(Gate* G);
void Objective(Gate* G);
DType LookUpTable(Gate* G);
DType DLookUpTable(Gate* G);
void ClearObjFixed(int frame);
void ClearDFrontierVisited(int frame);


//global variable


struct FrameWire
{
	vector<Wire*> FrameWires;
};


struct Circuit
{
	vector<Wire*> CWire;
	FrameWire FWires[10];
	vector<Gate*> CGate;
	vector<Wire*> InputWires;
	vector<Wire*> OutputWires;
	vector<Wire*> S_InputWires;
	vector<Wire*> S_OutputWires;
	vector<Gate*> DFrontiers;
	Gate* FrontierGate;
};

Circuit C[10];



int InputSize=0;

int main(int argc, char **argv)
{
	//read inputfile
	ifstream inFile[argc-1];
	string FileName[argc-1];
	for (int i=0; i<argc-1; i++)
	{
		FileName[i] = string(argv[i+1]);
		inFile[i].open(FileName[i].c_str());
	}
	int FrameNum = argc -1;
	//string FileName = string(argv[1]);
	//ifstream inFile;
	//inFile.open(FileName.c_str());
	
	for (int frame=0; frame<FrameNum; frame++)
	{
		if (!inFile[frame].good())
		{
			cout << "ERROR in read_input() - cannot open " << FileName[frame] << " for reading." << endl;
		}
		
		string line;
		//store the inputfile into the circuit data structure
		while (getline(inFile[frame], line))
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
					C[frame].CWire.push_back(NewInput);
					C[frame].InputWires.push_back(NewInput);
					int num = NewInput->GetFrameNum();
					C[frame].FWires[num].FrameWires.push_back(NewInput);
				}
	   		}

			else if (word=="output")
			{
				while (sstr >> word)
				{
					Wire* NewOutput = new Wire(word, OUTPUT, 0, X);
					C[frame].CWire.push_back(NewOutput);
					C[frame].OutputWires.push_back(NewOutput);
					int num = NewOutput->GetFrameNum();
					C[frame].FWires[num].FrameWires.push_back(NewOutput);
				}
			}

			else if (word=="wire")
			{
				while (sstr >> word)
				{
					Wire* NewWire = new Wire(word, WIRE, 0, X);
					C[frame].CWire.push_back(NewWire);
					int num = NewWire->GetFrameNum();
					C[frame].FWires[num].FrameWires.push_back(NewWire);
				}	
			}
			else if (word=="time_frame_output")
			{
				while (sstr >> word)
				{
					Wire* NewWire = new Wire(word, S_OUTPUT, 0, X);
					C[frame].S_OutputWires.push_back(NewWire);
					C[frame].CWire.push_back(NewWire);
					int num = NewWire->GetFrameNum();
					C[frame].FWires[num].FrameWires.push_back(NewWire);
				}	
			}
			else if (word=="time_frame_input")
			{
				while (sstr >> word)
				{
					Wire* NewWire = new Wire(word, S_INPUT, 0, ONE);
					C[frame].S_InputWires.push_back(NewWire);
					C[frame].CWire.push_back(NewWire);
					int num = NewWire->GetFrameNum();
					C[frame].FWires[num].FrameWires.push_back(NewWire);
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

				for (unsigned i=0; i<C[frame].CWire.size(); i++)
				{
					if (C[frame].CWire[i]->GetWireName()==dff_input)
					{
						DffInputs.push_back(C[frame].CWire[i]);					
					}
					if (C[frame].CWire[i]->GetWireName()==dff_output)
					{
						DffOutput = C[frame].CWire[i];
					}
				}
		
				Gate* NewDFF = new Gate(dff_name, BUFFER, DffInputs, DffOutput);
				C[frame].CGate.push_back(NewDFF);
			
				for (unsigned i=0; i<C[frame].CWire.size(); i++)
				{
					if(C[frame].CWire[i]->GetWireName() == dff_output)
					{
						C[frame].CWire[i]->SetFanIn(NewDFF);	
					}
					if (C[frame].CWire[i]->GetWireName() == dff_input)
					{
						C[frame].CWire[i]->AddFanOut(NewDFF);
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

				for (unsigned i=0; i<C[frame].CWire.size(); i++)
				{
					if (C[frame].CWire[i]->GetWireName()==not_input)
					{
						NotInputs.push_back(C[frame].CWire[i]);				
					}
					if (C[frame].CWire[i]->GetWireName()==not_output)
					{
						NotOutput = C[frame].CWire[i];
					}
				}
			
				Gate* NewNot = new Gate(not_name, NOT, NotInputs, NotOutput);
				C[frame].CGate.push_back(NewNot);

				for (unsigned i=0; i<C[frame].CWire.size(); i++)
				{
					if(C[frame].CWire[i]->GetWireName() == not_output)
					{
						C[frame].CWire[i]->SetFanIn(NewNot);	
					}
					if (C[frame].CWire[i]->GetWireName() == not_input)
					{
						C[frame].CWire[i]->AddFanOut(NewNot);
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

				for (unsigned i=0; i<C[frame].CWire.size(); i++)
				{
					if(C[frame].CWire[i]->GetWireName()==and_output)
					{
						AndOutput = C[frame].CWire[i];
						break;
					}
				}

				while (sstr >> and_input)
				{	
					for (unsigned i=0; i<C[frame].CWire.size(); i++)
					{
						if (C[frame].CWire[i]->GetWireName()==and_input)
						{
							AndInputs.push_back(C[frame].CWire[i]);
						}
					}
				}
				
				int size = AndInputs.size();
	

				Gate* NewAnd = new Gate(and_name, AND, AndInputs, AndOutput);
				C[frame].CGate.push_back(NewAnd);
				for (int j=0; j<size; j++)
				{
					and_input = AndInputs[j]->GetWireName();
					for (unsigned i=0; i<C[frame].CWire.size(); i++)
					{
						if(C[frame].CWire[i]->GetWireName() == and_input)
						{
							C[frame].CWire[i]->AddFanOut(NewAnd);	
						}
					}
				}
					
				for (unsigned i=0; i<C[frame].CWire.size(); i++)
				{
					if (C[frame].CWire[i]->GetWireName() == and_output)
					{
						C[frame].CWire[i]->SetFanIn(NewAnd);
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
	
				for (unsigned i=0; i<C[frame].CWire.size(); i++)
				{
					if(C[frame].CWire[i]->GetWireName()==or_output)
					{
						OrOutput = C[frame].CWire[i];
						break;
					}
				}

				while (sstr >> or_input)
				{	
					for (unsigned i=0; i<C[frame].CWire.size(); i++)
					{
						if (C[frame].CWire[i]->GetWireName()==or_input)
						{
							OrInputs.push_back(C[frame].CWire[i]);
						}
					}
				}
					
				int size = OrInputs.size();
	
				Gate* NewOr = new Gate(or_name, OR, OrInputs, OrOutput);
				C[frame].CGate.push_back(NewOr);
				for (int j=0; j<size; j++)
				{
					or_input = OrInputs[j]->GetWireName();
					for (unsigned i=0; i<C[frame].CWire.size(); i++)
					{
						if(C[frame].CWire[i]->GetWireName() == or_input)
						{
							C[frame].CWire[i]->AddFanOut(NewOr);	
						}
					}
				}
					
				for (unsigned i=0; i<C[frame].CWire.size(); i++)
				{
					if (C[frame].CWire[i]->GetWireName() == or_output)
					{
						C[frame].CWire[i]->SetFanIn(NewOr);
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
	
				for (unsigned i=0; i<C[frame].CWire.size(); i++)
				{
					if(C[frame].CWire[i]->GetWireName()==nor_output)
					{
						NorOutput = C[frame].CWire[i];
						break;
					}
				}

				while (sstr >> nor_input)
				{	
					for (unsigned i=0; i<C[frame].CWire.size(); i++)
					{
						if (C[frame].CWire[i]->GetWireName()==nor_input)
						{
							NorInputs.push_back(C[frame].CWire[i]);
						}
					}
				}
				
				int size = NorInputs.size();
	
	
				Gate* NewNor = new Gate(nor_name, NOR, NorInputs, NorOutput);
				C[frame].CGate.push_back(NewNor);
				for (int j=0; j<size; j++)
				{
					nor_input = NorInputs[j]->GetWireName();
					for (unsigned i=0; i<C[frame].CWire.size(); i++)
					{
						if(C[frame].CWire[i]->GetWireName() == nor_input)
						{
							C[frame].CWire[i]->AddFanOut(NewNor);	
						}
					}
				}
					
				for (unsigned i=0; i<C[frame].CWire.size(); i++)
				{
					if (C[frame].CWire[i]->GetWireName() == nor_output)
					{
						C[frame].CWire[i]->SetFanIn(NewNor);
						//cout<<CWire[i]->GetFanIn()->GetGateName()<<endl;
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

				for (unsigned i=0; i<C[frame].CWire.size(); i++)
				{
					if(C[frame].CWire[i]->GetWireName()== nand_output)
					{
						NandOutput = C[frame].CWire[i];
						break;
					}
				}

				while (sstr >> nand_input)
				{	
					for (unsigned i=0; i<C[frame].CWire.size(); i++)
					{
						if (C[frame].CWire[i]->GetWireName()==nand_input)
						{
							NandInputs.push_back(C[frame].CWire[i]);
						}
					}
				}
				
				int size = NandInputs.size();
	
	
				Gate* NewNand = new Gate(nand_name, NAND, NandInputs, NandOutput);
				C[frame].CGate.push_back(NewNand);
				for (int j=0; j<size; j++)
				{
					nand_input = NandInputs[j]->GetWireName();
					for (unsigned i=0; i<C[frame].CWire.size(); i++)
					{
						if(C[frame].CWire[i]->GetWireName() == nand_input)
						{
							C[frame].CWire[i]->AddFanOut(NewNand);	
						}
					}
				}
					
				for (unsigned i=0; i<C[frame].CWire.size(); i++)
				{
					if (C[frame].CWire[i]->GetWireName() == nand_output)
					{
						C[frame].CWire[i]->SetFanIn(NewNand);
						//cout<<CWire[i]->GetFanIn()->GetGateName()<<endl;
					}
				}
			}
		}
  
		cout << "file " << FileName << " successfully read." << endl;
		inFile[frame].close();
	
		/*--------------------for test--------------------*/
		//print all the Gate read from input file 
		int GateSize=C[frame].CGate.size();
		for (int i=0; i<GateSize; i++)
		{
			C[frame].CGate[i]->PrintGate();
			for (int j=0; j<C[frame].CGate[i]->GetInputSize(); j++)
			{
				cout<<"input"<<j<<"="<<(C[frame].CGate[i]->GetInputs())[j]->GetWireName()<<", ";
			}
			cout<<"output = "<<C[frame].CGate[i]->GetOutput()->GetWireName()<<endl;
		}
		//print all the Wire read from input file
		int WireSize = C[frame].CWire.size();
		for (int i=0; i<WireSize; i++)
		{
			C[frame].CWire[i]->PrintWire();
		}
		/*--------------------for test--------------------*/


		
	}


	
		int frame = 0;
		int TestNumber = 0;
		// PODEM
		// for each wire we will generate 
		// s-a-0 fault and s-a-1 fault

		int WireSize = C[frame].FWires[0].FrameWires.size();
		for (int i=0; i<WireSize; i++)
		{
			//intialize and set s-a-1 fault
			Initialize(frame);
			ClearObjFixed(frame);
			ClearDFrontierVisited(frame);
			//clear DFrontier List
			while (!C[frame].DFrontiers.empty())
			{
				C[frame].DFrontiers.pop_back();
			}
			cout<<"@@  Wire "<<C[frame].CWire[i]->GetWireName()<<" is the fault site. It's s-a-0 fault."<<endl;


			string WireHalfName = C[frame].FWires[0].FrameWires[i]->GetHalfName();
			/*--------------------for test--------------------*/
			cout<<"HalfName is "<<WireHalfName<<endl;
			/*--------------------for test--------------------*/
			for (int FNum = 0; FNum < frame+1; FNum++)
			{
				if (FNum!=frame)
				{
					for (int j=0; j<WireSize; j++)
					{
						if (C[frame].FWires[FNum].FrameWires[j]->GetHalfName()==WireHalfName)
						{
							C[frame].FWires[FNum].FrameWires[j]->SetStuck(true, ZERO);
							C[frame].FWires[FNum].FrameWires[j]->SetFixed(true);
						}
					}
				}
				else
				{
					for (int j=0; j<WireSize; j++)
					{
						if (C[frame].FWires[FNum].FrameWires[j]->GetHalfName()==WireHalfName)
						{
							C[frame].FWires[FNum].FrameWires[j]->SetStuck(true, D);
							C[frame].FWires[FNum].FrameWires[j]->SetFixed(true);
						}
					}
				}
			} 
	
			//C[frame].CWire[i]->SetStuck(true,D);
			//C[frame].CWire[i]->SetFixed(true);

			// Initial State must be all 0s.
			for (unsigned m=0; m<C[frame].S_InputWires.size(); m++)
			{
				C[frame].S_InputWires[m]->SetValue(ZERO);
				C[frame].S_InputWires[m]->SetFixed(true);
			}

			if (C[frame].CWire[i]->GetWireType()==INPUT)
			{
				C[frame].CWire[i]->SetBTVisited(true);
			}

			//Activate fault
			//Implication to the back
		
			if ((C[frame].CWire[i]->GetWireType()!=INPUT)&&(C[frame].CWire[i]->GetWireType()!=S_INPUT))
			{
				Gate* DBack = C[frame].CWire[i]->GetFanIn();
				ImplyBackward(DBack);
			}
		
			//find D-frontier
			//imply forward if there are NOT gates
			//and then update the frontier wire
			Wire* PodemWire = C[frame].CWire[i];
			if (PodemWire->GetWireType()!=S_INPUT)
			{
				if ((PodemWire->GetWireType()!=OUTPUT)&&(PodemWire->GetWireType()!=S_OUTPUT))
				{
					vector<Gate*> DFront = C[frame].CWire[i]->GetFanOut();
					PodemWire = ImplyForward(DFront, frame);	
				}
			

				//for test
				//PodemWire->PrintWire();
				/*--------------------for test--------------------
				//print all the Wire read from input file
				int WireSize = CWire.size();
				for (int m=0; m<WireSize; m++)
				{
					CWire[m]->PrintWire();
				}
				--------------------for test--------------------*/
	
				if(PODEM(PodemWire, frame)==true)
				{	
					TestNumber++;
					cout<<endl;
					cout<<"@@@ Wire "<<C[frame].CWire[i]->GetWireName()<<"/0 has test vector"<<endl;
					cout<<endl;
				}
				//Do PODEM
				else
				{
					//cout<<endl;
					//cout<<"fail!"<<endl;
					//cout<<endl;
					if (!C[frame].DFrontiers.empty())
					{
						C[frame].DFrontiers.pop_back();					
					}
					ClearObjFixed(frame);
					//cout<<"D SIZE = "<<DFrontiers.size()<<endl;
					if (!C[frame].DFrontiers.empty())
					{
						for (unsigned m=0; m<C[frame].DFrontiers.back()->GetInputs().size(); m++)
						{
							if ((C[frame].DFrontiers.back()->GetInputs()[m]->GetValue()==D)||(C[frame].DFrontiers.back()->GetInputs()[m]->GetValue()==DNOT))
							{
								PodemWire = C[frame].DFrontiers.back()->GetInputs()[m];
								break;
							}
						}
					}
					bool success=false;
					
					while (!C[frame].DFrontiers.empty())
					{
					
						if(PODEM(PodemWire, frame)==true)
						{	
							TestNumber++;
							cout<<endl;
							cout<<"@@@ Wire "<<C[frame].CWire[i]->GetWireName()<<"/0 has test vector"<<endl;
							cout<<endl;
							success = true;
							break;
						}
						else
						{
							//cout<<endl;
							//cout<<"fail!"<<endl;
							//cout<<endl;
							if (!C[frame].DFrontiers.empty())
							{
								C[frame].DFrontiers.pop_back();
							}
							ClearObjFixed(frame);
							//cout<<"D SIZE = "<<DFrontiers.size()<<endl;
							if (!C[frame].DFrontiers.empty())
							{
								for (unsigned m=0; m<C[frame].DFrontiers.back()->GetInputs().size(); m++)
								{
									if ((C[frame].DFrontiers.back()->GetInputs()[m]->GetValue()==D)||(C[frame].DFrontiers.back()->GetInputs()[m]->GetValue()==DNOT))
									{
										PodemWire = C[frame].DFrontiers.back()->GetInputs()[m];
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
						cout<<"@@@ Wire "<<C[frame].CWire[i]->GetWireName()<<"/0 has no test vector"<<endl;
						cout<<endl;
					}
				}		
			}


			/*---------------------------------------------------------------------------------------------------------------*/
				//intialize and set s-a-1 fault
			Initialize(frame);
			ClearObjFixed(frame);
			ClearDFrontierVisited(frame);

			while (!C[frame].DFrontiers.empty())
			{
				C[frame].DFrontiers.pop_back();
			}
			cout<<endl<<"@@  Wire "<<C[frame].CWire[i]->GetWireName()<<" is the fault site. It's s-a-1 fault."<<endl;



			//string WireHalfName = C[frame].FWires[0].FrameWires[i]->GetHalfName();
			/*--------------------for test--------------------*/
			cout<<WireHalfName<<endl;
			/*--------------------for test--------------------*/
			for (int FNum = 0; FNum < frame+1; FNum++)
			{
				
				if (FNum!=frame)
				{
					for (int j=0; j<WireSize; j++)
					{
						if (C[frame].FWires[FNum].FrameWires[j]->GetHalfName()==WireHalfName)
						{
							C[frame].FWires[FNum].FrameWires[j]->SetStuck(true, ZERO);
							C[frame].FWires[FNum].FrameWires[j]->SetFixed(true);
						}
					}
				}
				else
				{
					
					for (int j=0; j<WireSize; j++)
					{

						if (C[frame].FWires[FNum].FrameWires[j]->GetHalfName()==WireHalfName)
						{
							C[frame].FWires[FNum].FrameWires[j]->SetStuck(true, D);
							C[frame].FWires[FNum].FrameWires[j]->SetFixed(true);
						}
					}
				}
			} 
			//C[frame].CWire[i]->SetStuck(true,DNOT);
			//C[frame].CWire[i]->SetFixed(true);

			// Initial State must be all 0s.
			for (unsigned m=0; m<C[frame].S_InputWires.size(); m++)
			{
				C[frame].S_InputWires[m]->SetValue(ZERO);
				C[frame].S_InputWires[m]->SetFixed(true);
			}

			if (C[frame].CWire[i]->GetWireType()==INPUT)
			{
				C[frame].CWire[i]->SetBTVisited(true);
			}

			//Activate fault
			//Implication to the back
		
			if ((C[frame].CWire[i]->GetWireType()!=INPUT)&&(C[frame].CWire[i]->GetWireType()!=S_INPUT))
			{
				Gate* DBack = C[frame].CWire[i]->GetFanIn();
				ImplyBackward(DBack);
			}

			//find D-frontier
			//imply forward if there are NOT gates
			//and then update the frontier wire
			PodemWire = C[frame].CWire[i];
			if (PodemWire->GetWireType()!=S_INPUT)
			{
				if ((PodemWire->GetWireType()!=OUTPUT)&&(PodemWire->GetWireType()!=S_OUTPUT))
				{
					vector<Gate*> DFront = C[frame].CWire[i]->GetFanOut();
					PodemWire = ImplyForward(DFront, frame);	
				}
	

				//for test
				//PodemWire->PrintWire();
				/*--------------------for test--------------------
				//print all the Wire read from input file
				WireSize = CWire.size();
				for (int m=0; m<WireSize; m++)
				{
					CWire[m]->PrintWire();
				}
				--------------------for test--------------------*/
	
				//clear DFrontier List

				if(PODEM(PodemWire, frame)==true)
				{	
					TestNumber++;
					cout<<endl;
					cout<<endl<<"@@@ Wire "<<C[frame].CWire[i]->GetWireName()<<"/1 has test vector"<<endl;
					cout<<endl;
				}
				//Do PODEM
				else
				{
					if (!C[frame].DFrontiers.empty())			
					{
						C[frame].DFrontiers.pop_back();					
					}
					ClearObjFixed(frame);
		
					if (!C[frame].DFrontiers.empty())
					{
						for (unsigned m=0; m<C[frame].DFrontiers.back()->GetInputs().size(); m++)
						{
							if ((C[frame].DFrontiers.back()->GetInputs()[m]->GetValue()==D)||(C[frame].DFrontiers.back()->GetInputs()[m]->GetValue()==DNOT))
							{
								PodemWire = C[frame].DFrontiers.back()->GetInputs()[m];
								break;
							}
						}
					}
					bool success=false;
					
					while (!C[frame].DFrontiers.empty())
					{
						
						if(PODEM(PodemWire,frame)==true)
						{	
							TestNumber++;
							cout<<endl;
							cout<<"@@@ Wire "<<C[frame].CWire[i]->GetWireName()<<"/1 has test vector"<<endl;
							cout<<endl;
							success = true;
							break;
						}
						else
						{
	
							if (!C[frame].DFrontiers.empty())
							{
								C[frame].DFrontiers.pop_back();
							}
							ClearObjFixed(frame);
		
							if (!C[frame].DFrontiers.empty())
							{
								for (unsigned m=0; m<C[frame].DFrontiers.back()->GetInputs().size(); m++)
								{
									if ((C[frame].DFrontiers.back()->GetInputs()[m]->GetValue()==D)||(C[frame].DFrontiers.back()->GetInputs()[m]->GetValue()==DNOT))
									{
										PodemWire = C[frame].DFrontiers.back()->GetInputs()[m];
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
						cout<<"@@@ Wire "<<C[frame].CWire[i]->GetWireName()<<"/1 has no test vector"<<endl;
						cout<<endl;
					}
				}
			}
		}
		return 0;
}



//functions

bool PODEM(Wire* W, int frame)
{
	//if Stuck at fault is at primary output
	//we don't need to generate test vector
	cout<<"000000000000000000000000000000000"<<endl;
	Wire* CurrentWire = W;	
	int WireSize = C[frame].CWire.size();
	cout<<"wire size: "<<WireSize<<endl;
	cout<<"CurrentWire: "<<CurrentWire->GetWireName()<<endl;

	/*--------------------for test--------------------*/
	//print all the Gate read from input file 
	cout<<endl;
	cout<<"-----------------------------PODOM--------------------------------"<<endl;

	cout<<"CurrentWire = ";
	CurrentWire->PrintWire();
	cout<<endl;
	/*--------------------for test--------------------*/
		
	if ((CurrentWire->GetWireType()==OUTPUT)&&(CurrentWire->GetValue()!=X)&&(CurrentWire->GetValue()!=ZERO)&&(CurrentWire->GetValue()!=ONE)) 
		return true;
	else if (CurrentWire->GetWireType()==S_OUTPUT)
		return false;
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
		int GateSize=C[frame].CGate.size();
		for (int i=0; i<GateSize; i++)
		{
			C[frame].CGate[i]->PrintGate();
			for (int j=0; j<C[frame].CGate[i]->GetInputSize(); j++)
			{
				cout<<"input"<<j<<"="<<(C[frame].CGate[i]->GetInputs())[j]->GetWireName()<<", ";
			}
			cout<<"output = "<<C[frame].CGate[i]->GetOutput()->GetWireName()<<", DFrontierVisited: "<<C[frame].CGate[i]->GetDFrontierVisited()<<endl;
		}

		/*-------------------for test-------------------*/

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
				C[frame].DFrontiers.push_back(FrontierGates[i]);
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
				C[frame].DFrontiers.push_back(NotLists.front());
				NotLists.front()->SetDFrontierVisited(true);
				
			}	
			NotLists.erase(NotLists.begin());
		}

		//If No DFrontier, untestable
		//return false

		if (C[frame].DFrontiers.size()==0)
		{
			return false;
		}

		/*--------------------for test--------------------*/
		cout<<"C[frame].DFrontiers are: ";
		for (unsigned i=0; i<C[frame].DFrontiers.size(); i++)
		{
			cout<<C[frame].DFrontiers[i]->GetGateName()<<",";
		}	
		cout<<endl;
		/*--------------------for test--------------------*/
		
		//update current wire and frontier gate
		C[frame].FrontierGate = C[frame].DFrontiers.back();
		for (unsigned m=0; m<C[frame].FrontierGate->GetInputs().size(); m++)
		{
			if ((C[frame].FrontierGate->GetInputs()[m]->GetValue()==D)||(C[frame].FrontierGate->GetInputs()[m]->GetValue()==DNOT))
			{
				CurrentWire = C[frame].FrontierGate->GetInputs()[m];
				break;
			}
		}
		//C[frame].DFrontiers.erase(C[frame].DFrontiers.begin());

	

		/*--------------------for test--------------------*/
		cout<<"{NEW} Frontier Gate is:";	
		C[frame].FrontierGate->PrintGate();
		cout<<endl;
		/*--------------------for test--------------------*/

		//objective
		Objective(C[frame].FrontierGate);
		C[frame].FrontierGate->GetOutput()->SetValue(LookUpTable(C[frame].FrontierGate));

		/*--------------------for test--------------------*/
		cout<<"-------after objective---------"<<endl;
		for (int m=0; m<WireSize; m++)
		{
			C[frame].CWire[m]->PrintWire();
		}
		cout<<endl;
		/*--------------------for test--------------------*/

		//Backtrace
		Wire* BTResult = Backtrace(C[frame].FrontierGate);
		/*--------------------for test--------------------*/
		cout<<"-------after backtrace---------"<<endl;
		for (int m=0; m<WireSize; m++)
		{
			C[frame].CWire[m]->PrintWire();
		}	
		/*--------------------for test--------------------*/

		

		//if all the Backtrace is done, current wire change 
		//and frontier gate changes
		if(BTResult==NULL)
		{
			//if the frontier gate is not gate
			//we need to propagate it further
			//until the frontier is not a Not gate.
			

			CurrentWire = C[frame].FrontierGate->GetOutput();

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
			cout<<"C[frame].DFrontiers are: ";
			for (unsigned i=0; i<C[frame].DFrontiers.size(); i++)
			{
				cout<<C[frame].DFrontiers[i]->GetGateName()<<",";
			}	
			cout<<endl;
			/*--------------------for test--------------------*/

			C[frame].DFrontiers.pop_back();

		}


		//ImplyForward BTResult to see if there is a contradiction
		if (InputImplyForward(frame))
		{
			/*--------------------for test--------------------*/
			cout<<"-------after imply---------"<<endl;
			for (int m=0; m<WireSize; m++)
			{
				C[frame].CWire[m]->PrintWire();
			}
			/*--------------------for test--------------------*/

			if (PODEM(CurrentWire,frame)==true) return true;
		}
		if (BTResult==NULL)
		{
			//cout<<"BTResult = null"<<endl;
			//cout<<"D Size = "<<C[frame].DFrontiers.size()<<endl;
			return false;
		}

		//implyForward BTResult' to see if there is a contradiction
		if ((BTResult!=NULL)&&((BTResult->GetFixed()==true)||(BTResult->GetObjFixed()==true)))
			{return false;}
		if(BTResult->GetValue()==ONE)
			{BTResult->SetValue(ZERO);}
		else if (BTResult->GetValue()==ZERO)
			{BTResult->SetValue(ONE);}
		/*--------------------for test--------------------*/
		cout<<"---fail, we need to backtrack.---"<<endl;
		for (int m=0; m<WireSize; m++)
		{	
			C[frame].CWire[m]->PrintWire();
		}
		/*--------------------for test--------------------*/
		if (BTResult!=NULL)
		{	
			if(InputImplyForward(frame))
			{
				/*--------------------for test--------------------*/
				cout<<"-------after imply---------"<<endl;
				for (int m=0; m<WireSize; m++)
				{
					C[frame].CWire[m]->PrintWire();
				}
				/*--------------------for test--------------------*/
	
				if (PODEM(CurrentWire, frame)==true) return true;
			}
		
			//Imply BTResult= X
			BTResult->SetValue(X);
			BTResult->SetFixed(false);
			BTResult->SetBTVisited(false);
			InputImplyForward(frame);
		}
		return false;
	}
}


//set all the wire to X
void Initialize(int frame)
{
	int size = C[frame].CWire.size();
	for (int i=0; i<size; i++)
	{
		C[frame].CWire[i]->initialize();
	}
}

void ClearObjFixed(int frame)
{
	int size = C[frame].CWire.size();
	for (int i=0; i<size; i++)
	{
		C[frame].CWire[i]->SetObjFixed(false);
	}
}

void ClearDFrontierVisited(int frame)
{
	int size = C[frame].CGate.size();
	for (int i=0; i<size; i++)
	{
		C[frame].CGate[i]->SetDFrontierVisited(false);
	}
}


void ImplyBackward(Gate* G)
{
	vector<Gate*> gates;
	gates.push_back(G);
	while (gates.size()!=0)
	{
		//gates.front()->PrintGate();
		//cout<<endl;
		if (gates.front()->GetGateType()==NOT)
		{
			if (gates.front()->GetOutput()->GetValue()==ONE)
			{
				(gates.front()->GetInputs())[0]->SetValue(ZERO);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if ((gates.front()->GetInputs()[0]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[0]->GetWireType()!=S_INPUT))
				{
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}

			}
			else if (gates.front()->GetOutput()->GetValue()==ZERO)
			{
				(gates.front()->GetInputs())[0]->SetValue(ONE);
				//gates.front()->SetVisited(true);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if ((gates.front()->GetInputs()[0]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[0]->GetWireType()!=S_INPUT))
				{	
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==D)
			{	
				(gates.front()->GetInputs())[0]->SetValue(ZERO);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if ((gates.front()->GetInputs()[0]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[0]->GetWireType()!=S_INPUT))
				{
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==DNOT)
			{	
				(gates.front()->GetInputs())[0]->SetValue(ONE);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if ((gates.front()->GetInputs()[0]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[0]->GetWireType()!=S_INPUT))
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
				if ((gates.front()->GetInputs()[0]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[0]->GetWireType()!=S_INPUT))
				{
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}

			}
			else if (gates.front()->GetOutput()->GetValue()==ZERO)
			{
				(gates.front()->GetInputs())[0]->SetValue(ZERO);
				//gates.front()->SetVisited(true);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if ((gates.front()->GetInputs()[0]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[0]->GetWireType()!=S_INPUT))
				{	
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==D)
			{	
				(gates.front()->GetInputs())[0]->SetValue(ONE);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if ((gates.front()->GetInputs()[0]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[0]->GetWireType()!=S_INPUT))
				{
					gates.push_back(gates.front()->GetInputs()[0]->GetFanIn());
				}
			}
			else if (gates.front()->GetOutput()->GetValue()==DNOT)
			{	
				(gates.front()->GetInputs())[0]->SetValue(ZERO);
				gates.front()->GetInputs()[0]->SetFixed(true);
				if ((gates.front()->GetInputs()[0]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[0]->GetWireType()!=S_INPUT))
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
					if ((gates.front()->GetInputs()[i]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[i]->GetWireType()!=S_INPUT))
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
					if ((gates.front()->GetInputs()[i]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[i]->GetWireType()!=S_INPUT))
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
					if ((gates.front()->GetInputs()[i]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[i]->GetWireType()!=S_INPUT))
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
					if ((gates.front()->GetInputs()[i]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[i]->GetWireType()!=S_INPUT))
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
					if ((gates.front()->GetInputs()[i]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[i]->GetWireType()!=S_INPUT))
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
					if ((gates.front()->GetInputs()[i]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[i]->GetWireType()!=S_INPUT))
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
					if ((gates.front()->GetInputs()[i]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[i]->GetWireType()!=S_INPUT))
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
					if ((gates.front()->GetInputs()[i]->GetWireType()!=INPUT)&&(gates.front()->GetInputs()[i]->GetWireType()!=S_INPUT))
					{
						gates.push_back((gates.front()->GetInputs())[i]->GetFanIn());
					}
				}
			}
		}
		gates.erase(gates.begin());
	}
}


Wire* ImplyForward(vector<Gate*> Gs, int frame)
{
	vector<Gate*> gates = Gs;
	Wire* NewWire = NULL;
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
				vector<Gate*> outputs = gates.front()->GetOutput()->GetFanOut();
				gates.insert(gates.end(), outputs.begin(), outputs.end());
			}
			else
			{
				return gates.front()->GetOutput();
			}
		}
		else if (gates.front()->GetGateType() == BUFFER)
		{
			gates.front()->GetOutput()->SetValue(LookUpTable(gates.front()));
			gates.front()->GetOutput()->SetFixed(true);
			if (gates.front()->GetOutput()->GetWireType()!=OUTPUT)
			{
				vector<Gate*> outputs = gates.front()->GetOutput()->GetFanOut();
				gates.insert(gates.end(), outputs.begin(), outputs.end());
			}
			else
			{
				return gates.front()->GetOutput();
			}
		}
		else
		{
			C[frame].DFrontiers.push_back(gates.front());
			gates.front()->SetDFrontierVisited(true);
		}
		gates.erase(gates.begin());	
	}

	if (!C[frame].DFrontiers.empty())
	{
		int size = C[frame].DFrontiers.back()->GetInputs().size();
		vector<Wire*> inputs = C[frame].DFrontiers.back()->GetInputs();
		for (int i=0; i<size; i++)
		{
			if ((inputs[i]->GetValue()==D)||(inputs[i]->GetValue()==DNOT))
			{
				NewWire = inputs[i];
				break;
			}
		}
	}
	return NewWire;
}


//from primary input to forward gates
//check if there is a contradiction
bool InputImplyForward(int frame)
{
	vector<Wire*> wires = C[frame].InputWires;
	for (unsigned i=0; i<C[frame].CWire.size(); i++)
	{
		C[frame].CWire[i]->SetInternalBTVisited(false);
	}
	
	while (wires.size()!=0)
	{
		vector<Gate*> fanout;
		if (wires.front()->GetWireType()!=OUTPUT)
		{
			fanout = wires.front()->GetFanOut();
		}
		for (unsigned i=0; i<fanout.size(); i++)
		{
			cout<<"gate: "<<fanout[i]->GetGateName()<<", BTVisited = "<<fanout[i]->GetOutput()->GetInternalBTVisited()<<endl;
			if ( (LookUpTable(fanout[i])!=X)
				&&(  ! (  (fanout[i]->GetOutput()->GetValue()==LookUpTable(fanout[i]))
				||        (fanout[i]->GetOutput()->GetValue()==DLookUpTable(fanout[i])) ))
				&&((fanout[i]->GetOutput()->GetFixed()==true)||(fanout[i]->GetOutput()->GetObjFixed()==true)))
			{
				cout<<fanout[i]->GetGateName()<<" return false"<<endl;
				//
				return false;
			}
			else if ((fanout[i]->GetOutput()->GetFixed()==false)&&(fanout[i]->GetOutput()->GetObjFixed()==false))
			{
				cout<<"fanout is "<<fanout[i]->GetGateName()<<endl;
				fanout[i]->GetOutput()->SetValue(LookUpTable(fanout[i]));	
			}

			if (fanout[i]->GetOutput()->GetInternalBTVisited()==false)
			{
				fanout[i]->GetOutput()->SetInternalBTVisited(true);
				wires.push_back(fanout[i]->GetOutput());
			}
		}
		wires.erase(wires.begin());
	}

	bool success=false;
	for (unsigned i = 0; i<C[frame].OutputWires.size(); i++)
	{
		if((C[frame].OutputWires[i]->GetValue()== DNOT)||(C[frame].OutputWires[i]->GetValue()== D)||(C[frame].OutputWires[i]->GetValue()== X))
		{
			//cout<<"return true"<<endl;
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
		if (((G->GetInputs())[i]->GetWireType()==INPUT)||((G->GetInputs())[i]->GetWireType()==S_INPUT))
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
