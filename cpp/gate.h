#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum GType{NOT, AND, NAND, OR, NOR, DFF};

class Wire;

class Gate
{
	friend class Wire;

	private:
		string GName;
		GType Type;
		vector<Wire*> GInputs;
		Wire* GOutput;
		bool IsVisited;

	public:
		//constructor
		Gate(string name, GType type, vector<Wire*> inputs, Wire* output){GName = name; Type = type; GInputs = inputs; GOutput = output; IsVisited = false;}

		void initialize(){IsVisited = false;}
		
		//get the private value
		int GetInputSize(){return GInputs.size();}

		string GetGateName(){return GName;}

		GType GetGateType(){return Type;}

		Wire* GetOutput(){return GOutput;}

		bool GetVisited(){return IsVisited;}

		vector<Wire*> GetInputs(){return GInputs;}
		
		//Set the private value
		void SetVisited(bool isvisited){IsVisited = isvisited;}

		//print the value
		void PrintGate();
};
