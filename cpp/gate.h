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

		void Init(string name, GType type, vector<Wire*> inputs, Wire* output){GName = name; Type = type; GInputs = inputs; GOutput = output;}
		
		int GetInputSize(){return GInputs.size();}

		void PrintGate();

		string GetGateName(){return GName;}

		GType GetGateType(){return Type;}

		Wire* GetOutput(){return GOutput;}

		vector<Wire*> GetInputs(){return GInputs;}

		bool GetVisited(){return IsVisited;}
		
		void SetVisited(bool isvisited){IsVisited = isvisited;}
};
