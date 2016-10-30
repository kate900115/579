#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum GType{NOT, AND2, AND3, AND4, NAND2, NAND3, NAND4, OR2, OR3, OR4, NOR2, NOR3, NOR4, DFF};

class Gate
{
	private:
		string GName;
		GType Type;
		vector<string> GInputs;
		string GOutput;

	public:
		//constructor
		Gate(string name, GType type, vector<string> inputs, string output){GName = name; Type = type; GInputs = inputs; GOutput = output;}

		void Init(string name, GType type, vector<string> inputs, string output){GName = name; Type = type; GInputs = inputs; GOutput = output;}
		
		int GetInputSize(){return GInputs.size();}

		void PrintGate();

		string GetGateName(){return GName;}
};
