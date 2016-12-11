#include "wire.h"
#include <iostream>

using namespace std;

void Wire::PrintWire()
{
	cout<<"Name:"<<WName<<", Type:";
	if (Type==INPUT)
	{
		cout<<"primary input, Value=";
	}	
	else if (Type==OUTPUT)
	{
		cout<<"primary output, Value=";
	}
	else if (Type==WIRE)
	{
		cout<<"internal wire, Value=";
	}
	else if (Type==S_INPUT)
	{
		cout<<"secondary input, Value=";
	}
	else if (Type==S_OUTPUT)
	{
		cout<<"secondray output, Value=";
	}

	if (Value==ZERO)
	{
		cout<<"0";
	}
	else if(Value==ONE)
	{
		cout<<"1";
	}
	else if(Value==D)
	{
		cout<<"D";
	}
	else if(Value==DNOT)
	{
		cout<<"D'";
	}
	else if(Value==X)
	{
		cout<<"X";
	}
	else if(Value==ZERO_X)
	{
		cout<<"0/X";
	}
	else if(Value==ONE_X)
	{
		cout<<"1/X";
	}
	else if(Value==X_ZERO)
	{
		cout<<"X/1";
	}
	else if(Value==X_ONE)
	{
		cout<<"X/0";
	}

	cout<<", Is fixed= "<<IsFixed<<", IsObjFixed = "<<ObjFixed;
	
	if (FanInGate!=NULL)
	{
		cout<<", FanIn ="<<FanInGate->GetGateName();
	}
	if (FanOutGate.size()!=0)
	{	
		cout<<", FanOut =";
		for (unsigned i=0; i<FanOutGate.size(); i++)
		{
			cout<<FanOutGate[i]->GetGateName()<<", ";
		}
		cout<<endl;
	}
	else
	{
		cout<<endl;
	}
}	
