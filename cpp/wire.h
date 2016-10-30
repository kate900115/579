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
		DType Value;

	public:
		//constructor
		Wire (string name, WType type, bool is_stack, DType v){WName = name; Type = type; IsStack = is_stack; Value = v;}

		void PrintWire();

		void SetStack(bool is_stack, DType v){IsStack = is_stack; Value = v;}

		WType GetWireType(){return Type;}

		string GetWireName(){return WName;}

		
};
