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

	public:
		//constructor
		Gate(string name, GType type, vector<Wire*> inputs, Wire* output){GName = name; Type = type; GInputs = inputs; GOutput = output; }
		
		//get the private value
		int GetInputSize(){return GInputs.size();}

		string GetGateName(){return GName;}

		GType GetGateType(){return Type;}

		Wire* GetOutput(){return GOutput;}

		vector<Wire*> GetInputs(){return GInputs;}

		//print the value
		void PrintGate();
};
