#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum GType{NOT, AND, NAND, OR, NOR, BUFFER};

class Wire;

class Gate
{
	friend class Wire;

	private:
		string GName;
		GType Type;
		vector<Wire*> GInputs;
		Wire* GOutput;
		bool DFrontierVisited;

	public:
		//constructor
		Gate(string name, GType type, vector<Wire*> inputs, Wire* output)
		{
			GName = name; 
			Type = type; 
			GInputs = inputs; 
			GOutput = output; 
			DFrontierVisited = false;
		}
		
		//get the private value
		int GetInputSize(){return GInputs.size();}

		string GetGateName(){return GName;}

		GType GetGateType(){return Type;}

		Wire* GetOutput(){return GOutput;}

		vector<Wire*> GetInputs(){return GInputs;}

		bool GetDFrontierVisited(){return DFrontierVisited;}

		void SetDFrontierVisited(bool visited){DFrontierVisited = visited;}

		//print the value
		void PrintGate();
};
