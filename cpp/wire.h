#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "gate.h"

using namespace std;

//Define possible values inside the circuit
//{0,1,D,D',X, 0/X, 1/X, X/0, X/1}
enum DType {ZERO, ONE, D, DNOT, X, ZERO_X, ONE_X, X_ZERO, X_ONE};

//Define the type of the wire
enum WType {INPUT, OUTPUT, WIRE, S_INPUT, S_OUTPUT};

class Gate;

class Wire
{
	friend class Gate;

	private:
		string WName;
		WType Type;
		bool IsStuck;
		DType Value;
		Gate* FanInGate;
		vector<Gate*> FanOutGate;
		bool BTVisited;
		bool IsFixed;
		string FirstHalf;
		string HalfName;
		int FrameNum;

	public:
		//constructor
		Wire (string name, WType type, bool is_stuck, DType v)
		{
			WName = name; 
			Type = type; 
			IsStuck = is_stuck; 
			Value = v; 
			FanInGate = NULL; 
			FanOutGate = vector<Gate*>(); 
			BTVisited = false;
			IsFixed= false;
			HalfName = name.substr(0, name.size()-3);
			string a = name.substr(name.length()-1);
			FrameNum = atoi(a.c_str());
		}

		void initialize(){IsStuck = false; Value = X; BTVisited = false; IsFixed = false;}


		//get the private value
		WType GetWireType(){return Type;}

		string GetWireName(){return WName;}

		Gate* GetFanIn(){return FanInGate;}

		DType GetValue(){return Value;}

		vector<Gate*> GetFanOut(){return FanOutGate;}

		bool GetBTVisited(){return BTVisited;}

		bool GetFixed(){return IsFixed;}
		
		void SetFixed(bool fix){IsFixed = fix;}

		bool GetStuck(){return IsStuck;}
		
		//set the private value
		void SetValue(DType V){Value = V;}

		void SetStuck(bool is_stuck, DType v){IsStuck = is_stuck; Value = v;}

		void AddFanOut(Gate* NewGate){ FanOutGate.push_back(NewGate); }

		void SetFanIn(Gate* NewGate){ FanInGate = NewGate; }

		void SetBTVisited(bool is_visited){BTVisited = is_visited;}

		void PrintWire();
		
};
