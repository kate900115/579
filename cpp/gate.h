#include <iostream>
#include <string>
#include <vector>
#include "Wire.h"

using namespace std;

class Gate
{
	private:
		string GName;
		string GType;
		vector<Wire> GInputs;
		Wire GOutput;

	public:
		//constructor
		Gate(string name, string type, vector<Wire> inputs, Wire output){GName = name; GType = type; GInputs = inputs; GOutput = output;}
};
