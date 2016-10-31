#include "gate.h"
#include <iostream>

using namespace std;

void Gate::PrintGate()
{
	if (Type==NOT)
	{
		cout<<"Name: "<<GName<<", Type: NOT, ";
		int size =GInputs.size();
		for (int i=1; i<size+1; i++)
		{
			cout<<"input"<<i<<"="<<GInputs[i-1]<<", ";
		}
		cout<<"output = "<<GOutput<<endl;
	}
	else if (Type==DFF)
	{
		cout<<"Name: "<<GName<<", Type: DFF, ";
		int size =GInputs.size();
		for (int i=1; i<size+1; i++)
		{
			cout<<"input"<<i<<"="<<GInputs[i-1]<<", ";
		}
		cout<<"output = "<<GOutput<<endl;
	}
	else if ((Type==AND2)||(Type==AND3)||(Type==AND4))
	{
		cout<<"Name: "<<GName<<", Type: AND, ";
		int size =GInputs.size();
		for (int i=1; i<size+1; i++)
		{
			cout<<"input"<<i<<"="<<GInputs[i-1]<<", ";
		}
		cout<<"output = "<<GOutput<<endl;
	}
	else if ((Type==NAND2)||(Type==NAND3)||(Type==NAND4))
	{
		cout<<"Name: "<<GName<<", Type: NAND, ";
		int size =GInputs.size();
		for (int i=1; i<size+1; i++)
		{
			cout<<"input"<<i<<"="<<GInputs[i-1]<<", ";
		}
		cout<<"output = "<<GOutput<<endl;
	}
	else if ((Type==OR2)||(Type==OR3)||(Type==OR4))
	{
		cout<<"Name: "<<GName<<", Type: OR, ";
		int size =GInputs.size();
		for (int i=1; i<size+1; i++)
		{
			cout<<"input"<<i<<"="<<GInputs[i-1]<<", ";
		}
		cout<<"output = "<<GOutput<<endl;
	}
	else if ((Type==NOR2)||(Type==NOR3)||(Type==NOR4))
	{
		cout<<"Name: "<<GName<<", Type: NOR, ";
		int size =GInputs.size();
		for (int i=1; i<size+1; i++)
		{
			cout<<"input"<<i<<"="<<GInputs[i-1]<<", ";
		}
		cout<<"output = "<<GOutput<<endl;
	}
}	
