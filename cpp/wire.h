#include <iostream>
#include <vector>
#include <string>
#include "gate.h"

using namespace std;

//Define possible values inside the circuit
//{0,1,D,D',X, 0/X, 1/X, X/0, X/1}
enum DType {ZERO, ONE, D, DNOT, X, ZERO_X, ONE_X, X_ZERO, X_ONE};

//Define the type of the wire
enum WType {INPUT, OUTPUT, WIRE};

class Gate;

class Wire
{
	friend class Gate;

	private:
		string WName;
		WType Type;
		bool IsStack;
		DType Value;
		Gate* FanInGate;
		vector<Gate*> FanOutGate;
		bool BTVisited;

	public:
		//constructor
		Wire (string name, WType type, bool is_stack, DType v)
		{
			WName = name; 
			Type = type; 
			IsStack = is_stack; 
			Value = v; 
			FanInGate = NULL; 
			FanOutGate=vector<Gate*>(); 
			BTVisited = false;
		}

		void initialize(){IsStack = false; Value = X; BTVisited = false;}


		//get the private value
		WType GetWireType(){return Type;}

		string GetWireName(){return WName;}

		Gate* GetFanIn(){return FanInGate;}

		DType GetValue(){return Value;}

		vector<Gate*> GetFanOut(){return FanOutGate;}

		bool GetBTVisited(){return BTVisited;}
		
		//set the private value
		void SetValue(DType V){Value = V;}

		void SetStack(bool is_stack, DType v){IsStack = is_stack; Value = v;}

		void AddFanOut(Gate* NewGate){ FanOutGate.push_back(NewGate); }

		void SetFanIn(Gate* NewGate){ FanInGate = NewGate; }

		void SetBTVisited(bool is_visited){BTVisited = is_visited;}

		void PrintWire();
		
};
