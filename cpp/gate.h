#include <iostream>
#include <string>
#include <vector>
#include "Wire.h"

using namespace std;

enum GType{NOT, AND2, AND3, AND4, NAND2, NAND3, NAND4, OR2, OR3, OR4, NOR2, NOR3, NOR4, DFF};

class Gate
{
	private:
		string GName;
		string GType;
		vector<string> GInputs;
		string GOutput;

	public:
		//constructor
		Gate(string name, string type, vector<string> inputs, string output){GName = name; GType = type; GInputs = inputs; GOutput = output;}
		
		int GetInputSize(){return GInputs.size();}
};
