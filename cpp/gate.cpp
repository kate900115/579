#include "gate.h"
#include <iostream>
#include <string>

using namespace std;

void Gate::PrintGate()
{
	if (Type==NOT)
	{
		cout<<"Name: "<<GName<<", Type: NOT, ";
	//	int size =GInputs.size();
	//	for (int i=1; i<size+1; i++)
	//	{
	//		cout<<"input"<<i<<"="<<GInputs[i-1]<<", ";
	//	}
	//	cout<<"output = "<<GOutput<<endl;
	}
	else if (Type==BUFFER)
	{
		cout<<"Name: "<<GName<<", Type: DFF, ";
	//	int size =GInputs.size();
	//	for (int i=1; i<size+1; i++)
	//	{
	//		cout<<"input"<<i<<"="<<GInputs[i-1]<<", ";
	//	}
	//	cout<<"output = "<<GOutput<<endl;
	}
	else if (Type==AND)
	{
		cout<<"Name: "<<GName<<", Type: AND, ";
	//	int size =GInputs.size();
	//	for (int i=1; i<size+1; i++)
	//	{
	//		cout<<"input"<<i<<"="<<GInputs[i-1]<<", ";
	//	}
	//	cout<<"output = "<<GOutput<<endl;
	}
	else if (Type==NAND)
	{
		cout<<"Name: "<<GName<<", Type: NAND, ";
	//	int size =GInputs.size();
	//	for (int i=1; i<size+1; i++)
	//	{
	//		cout<<"input"<<i<<"="<<GInputs[i-1]<<", ";
	//	}
	//	cout<<"output = "<<GOutput<<endl;
	}
	else if (Type==OR)
	{
		cout<<"Name: "<<GName<<", Type: OR, ";
	//	int size =GInputs.size();
	//	for (int i=1; i<size+1; i++)
	//	{
	//		cout<<"input"<<i<<"="<<GInputs[i-1]<<", ";
	//	}
	//	cout<<"output = "<<GOutput<<endl;
	}
	else if (Type==NOR)
	{
		cout<<"Name: "<<GName<<", Type: NOR, ";
	//	int size =GInputs.size();
	//	for (int i=1; i<size+1; i++)
	//	{
	//		cout<<"input"<<i<<"="<<GInputs[i-1]<<", ";
	//	}
	//	cout<<"output = "<<GOutput<<endl;
	}
}	
