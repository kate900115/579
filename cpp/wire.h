#include <iostream>
#include <vector>
#include <string>
#include "DValue.h"

using namespace std;

class Wire
{
	private:
		string WName;
		string WType;
		bool IsStack;
		DValue value;

	public:
		//constructor
		Wire (string name, string type, bool is_stack, DValue v){WName = name; WType = type; IsStack = is_stack; value = v;}

		
};
