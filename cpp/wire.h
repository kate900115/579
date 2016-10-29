#include <iostream>
#include <vector>
#include <string>

using namespace std;

//Define possible values inside the circuit
//{0,1,D,D',X, 0/X, 1/X, X/0, X/1}
enum DType {ZERO, ONE, D, DNOT, X, ZERO_X, ONE_X, X_ZERO, X_ONE};

//Define the type of the wire
enum WType {INPUT, OUTPUT, WIRE};

class Wire
{
	private:
		string WName;
		WType Type;
		bool IsStack;
		DValue Value;

	public:
		//constructor
		Wire (string name, string type, bool is_stack, DValue v){WName = name; Type = type; IsStack = is_stack; Value = v;}

		
};
