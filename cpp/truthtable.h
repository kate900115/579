//giving input of the gate G,
//get output of gate G.
DType LookUpTable(Gate* G)
{
	if (G->GetGateType()==NOT)
	{
		DType InputValue = (G->GetInputs())[0]->GetValue();
		if (InputValue == D)
		{return DNOT;}
		if (InputValue == DNOT)
		{return D;}
		if (InputValue == ONE)
		{return ZERO;}
		if (InputValue == ZERO)
		{return ONE;}
		if (InputValue == X)
		{return X;}
	}
	else if (G->GetGateType()==AND)
	{
		vector<DType> InputValues;
		for (int i=0; i<G->GetInputSize(); i++)
		{
			if((G->GetInputs())[i]->GetValue()==ZERO)
			{return ZERO;}
			else if ((G->GetInputs())[i]->GetValue()==X)
			{return X;}
			else
			{
				InputValues.push_back((G->GetInputs())[i]->GetValue());
			}
		}	
		if (InputValues.size()==2)
		{	
			// input = 11, output = 1;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE))
			{return ONE;}
			//input = 1D, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D))
			{return D;}
			//input = D1, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE))
			{return D;}
			//input 1D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT))
			{return DNOT;}
			//input D'1, output = D'
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE))
			{return DNOT;}
			//input DD, output D
			else if ((InputValues[0]==D)&&(InputValues[1]==D))
			{return D;}
			//input D'D', output D'
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT))
			{return DNOT;}
			else 
			{return ZERO;}
		}
		if (InputValues.size()==3)
		{
			// input = 111, output = 1;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return ONE;}
			// input = 11D, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D))
			{return D;}
			// input = 1D1, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE))
			{return D;}
			// input = D11, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return D;}
			// input = 1DD, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return D;}
			// input = D1D, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D))
			{return D;}
			// input = DD1, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE))
			{return D;}
			// input = DDD, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return D;}
			// input = 11D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = 1D'1, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE))
			{return DNOT;}
			// input = D'11, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return DNOT;}
			// input = 1D'D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = D'1D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = D'D'1, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE))
			{return DNOT;}
			// input = D'D'D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return DNOT;}
			else
			{return ZERO;}			
			
		}
		if (InputValues.size()==4)
		{
			// input = 1111, output = 1;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return ONE;}
			// input = 111D, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return D;}
			// input = 11D1, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return D;}
			// input = 1D11, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return D;}
			// input = D111, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return D;}
			// input = 111D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = 11D'1, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 1D'11, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = D'111, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 11DD, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = 1D1D, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return D;}
			// input = D11D, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return D;}
			// input = D1D1, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return D;}
			// input = DD11, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return D;}
			// input = 1DD1, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return D;}
			// input = 11D'D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = 1D'1D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'11D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'1D'1, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = D'D'11, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 1D'D'1, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 1DDD, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = D1DD, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = DD1D, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return D;}
			// input = DDD1, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return D;}
			// input = 1D'D'D', output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D'1D'D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'D'1D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'D'D'1, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = DDDD, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = D'D'D'D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return DNOT;}
			else
			{return ZERO;}
		}
	}
	else if (G->GetGateType()==OR)
	{
		vector<DType> InputValues;
		for (int i=0; i<G->GetInputSize(); i++)
		{
			if((G->GetInputs())[i]->GetValue()==ONE)
			{return ONE;}
			else if ((G->GetInputs())[i]->GetValue()==X)
			{return X;}
			else
			{
				InputValues.push_back((G->GetInputs())[i]->GetValue());
			}
		}	
		if (InputValues.size()==2)
		{	
			// input = 00, output = 0;
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO))
			{return ZERO;}
			//input = 0D, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D))
			{return D;}
			//input = D0, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO))
			{return D;}
			//input 0D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT))
			{return DNOT;}
			//input D'0, output = D'
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO))
			{return DNOT;}
			//input DD, output D
			else if ((InputValues[0]==D)&&(InputValues[1]==D))
			{return D;}
			//input DD', output 1
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT))
			{return ONE;}
			//input D'D', output D'
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT))
			{return DNOT;}
			//input D'D, output 1
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D))
			{return ONE;}
		}
		if (InputValues.size()==3)
		{
			// input = OOO, output = 0;
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return ZERO;}
			// input = 00D, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D))
			{return D;}
			// input = 0D0, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO))
			{return D;}
			// input = D00, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return D;}
			// input = 0DD, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return D;}
			// input = D0D, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D))
			{return D;}
			// input = DD0, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO))
			{return D;}
			// input = DDD, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return D;}
			// input = 00D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = 0D'0, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO))
			{return DNOT;}
			// input = D'00, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return DNOT;}
			// input = 0D'D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = D'0D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT))
			{return DNOT;}
			// input = D'D'0, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO))
			{return DNOT;}
			// input = D'D'D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return DNOT;}
			else
			{return ONE;}			
			
		}
		if (InputValues.size()==4)
		{
			// input = 0000, output = 0;
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return ZERO;}
			// input = 000D, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return D;}
			// input = 00D0, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 0D00, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return D;}
			// input = D000, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 000D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = 00D'0, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 0D'00, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = D'000, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 00DD, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = 0D0D, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return D;}
			// input = D00D, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return D;}
			// input = D0D0, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return D;}
			// input = DD00, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 0DD0, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 00D'D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = 0D'0D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'00D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'0D'0, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = D'D'00, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 0D'D'0, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 0DDD, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = D0DD, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = DD0D, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return D;}
			// input = DDD0, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 0D'D'D', output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D'0D'D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'D'0D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return DNOT;}
			// input = D'D'D'0, output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = DDDD, output = D;
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = D'D'D'D', output = D';
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return DNOT;}
			else
			{return ONE;}
		}
	}
	else if (G->GetGateType()==NAND)
	{
		vector<DType> InputValues;
		for (int i=0; i<G->GetInputSize(); i++)
		{
			if((G->GetInputs())[i]->GetValue()==ZERO)
			{return ONE;}
			else if ((G->GetInputs())[i]->GetValue()==X)
			{return X;}
			else
			{
				InputValues.push_back((G->GetInputs())[i]->GetValue());
			}
		}	
		if (InputValues.size()==2)
		{	
			// input = 11, output = 0;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE))
			{return ZERO;}
			//input = 1D, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D))
			{return DNOT;}
			//input = D1, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE))
			{return DNOT;}
			//input 1D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT))
			{return D;}
			//input D'1, output = D
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE))
			{return D;}
			//input DD, output = D'
			else if ((InputValues[0]==D)&&(InputValues[1]==D))
			{return DNOT;}
			//input DD', output ONE
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT))
			{return ONE;}
			//input D'D', output D
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT))
			{return D;}
			//input D'D, output 1
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D))
			{return ONE;}
		}
		if (InputValues.size()==3)
		{
			// input = 111, output = 0;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return ZERO;}
			// input = 11D, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D))
			{return DNOT;}
			// input = 1D1, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE))
			{return DNOT;}
			// input = D11, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return DNOT;}
			// input = 1DD, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return DNOT;}
			// input = D1D, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D))
			{return DNOT;}
			// input = DD1, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE))
			{return DNOT;}
			// input = DDD, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return DNOT;}
			// input = 11D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT))
			{return D;}
			// input = 1D'1, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE))
			{return D;}
			// input = D'11, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return D;}
			// input = 1D'D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return D;}
			// input = D'1D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT))
			{return D;}
			// input = D'D'1, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE))
			{return D;}
			// input = D'D'D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return D;}
			else
			{return ONE;}			
			
		}
		if (InputValues.size()==4)
		{
			// input = 1111, output = 0;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return ZERO;}
			// input = 111D, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return DNOT;}
			// input = 11D1, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 1D11, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = D111, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 111D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return D;}
			// input = 11D'1, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return D;}
			// input = 1D'11, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return D;}
			// input = D'111, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return D;}
			// input = 11DD, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = 1D1D, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D11D, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D1D1, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = DD11, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 1DD1, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 11D'D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return D;}
			// input = 1D'1D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'11D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'1D'1, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return D;}
			// input = D'D'11, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==ONE))
			{return D;}
			// input = 1D'D'1, output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return D;}
			// input = 1DDD, output = D';
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D1DD, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ONE)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = DD1D, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ONE)&&(InputValues[3]==D))
			{return DNOT;}
			// input = DDD1, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ONE))
			{return DNOT;}
			// input = 1D'D'D', output = D;
			else if ((InputValues[0]==ONE)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = D'1D'D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ONE)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'D'1D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ONE)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'D'D'1, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ONE))
			{return D;}
			// input = DDDD, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D'D'D'D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return D;}
			else
			{return ONE;}
		}
	}
	else if (G->GetGateType()==NOR)
	{
		vector<DType> InputValues;
		for (int i=0; i<G->GetInputSize(); i++)
		{
			if((G->GetInputs())[i]->GetValue()==ONE)
			{return ZERO;}
			else if ((G->GetInputs())[i]->GetValue()==X)
			{return X;}
			else
			{
				InputValues.push_back((G->GetInputs())[i]->GetValue());
			}
		}	
		if (InputValues.size()==2)
		{	
			// input = 00, output = 1;
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO))
			{return ONE;}
			//input = 0D, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D))
			{return DNOT;}
			//input = D0, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO))
			{return DNOT;}
			//input 0D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT))
			{return D;}
			//input D'0, output = D
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO))
			{return D;}
			//input DD, output D'
			else if ((InputValues[0]==D)&&(InputValues[1]==D))
			{return DNOT;}
			//input DD', output 0
			else if ((InputValues[0]==D)&&(InputValues[1]==DNOT))
			{return ZERO;}
			//input D'D', output D
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT))
			{return D;}
			//input D'D, output 0
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==D))
			{return ZERO;}
		}
		if (InputValues.size()==3)
		{
			// input = OOO, output = 1;
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return ONE;}
			// input = 00D, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D))
			{return DNOT;}
			// input = 0D0, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO))
			{return DNOT;}
			// input = D00, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return DNOT;}
			// input = 0DD, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return DNOT;}
			// input = D0D, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D))
			{return DNOT;}
			// input = DD0, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO))
			{return DNOT;}
			// input = DDD, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D))
			{return DNOT;}
			// input = 00D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT))
			{return D;}
			// input = 0D'0, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO))
			{return D;}
			// input = D'00, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return D;}
			// input = 0D'D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return D;}
			// input = D'0D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT))
			{return D;}
			// input = D'D'0, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO))
			{return D;}
			// input = D'D'D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT))
			{return D;}
			else
			{return ZERO;}			
			
		}
		if (InputValues.size()==4)
		{
			// input = 0000, output = 1;
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return ONE;}
			// input = 000D, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return DNOT;}
			// input = 00D0, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 0D00, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = D000, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 000D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return D;}
			// input = 00D'0, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 0D'00, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return D;}
			// input = D'000, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 00DD, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = 0D0D, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D00D, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D0D0, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = DD00, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 0DD0, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 00D'D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return D;}
			// input = 0D'0D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'00D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'0D'0, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return D;}
			// input = D'D'00, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 0D'D'0, output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return D;}
			// input = 0DDD, output = D';
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D0DD, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==ZERO)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = DD0D, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==ZERO)&&(InputValues[3]==D))
			{return DNOT;}
			// input = DDD0, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==ZERO))
			{return DNOT;}
			// input = 0D'D'D', output = D;
			else if ((InputValues[0]==ZERO)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return D;}
			// input = D'0D'D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==ZERO)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'D'0D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==ZERO)&&(InputValues[3]==DNOT))
			{return D;}
			// input = D'D'D'0, output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==ZERO))
			{return D;}
			// input = DDDD, output = D';
			else if ((InputValues[0]==D)&&(InputValues[1]==D)&&(InputValues[2]==D)&&(InputValues[3]==D))
			{return DNOT;}
			// input = D'D'D'D', output = D;
			else if ((InputValues[0]==DNOT)&&(InputValues[1]==DNOT)&&(InputValues[2]==DNOT)&&(InputValues[3]==DNOT))
			{return D;}
			else
			{return ZERO;}
		}
	}
	return X;
}



DType DLookUpTable(Gate* G)
{
	if (G->GetGateType()==AND)
	{
		vector<DType> InputValues;
		for (int i=0; i<G->GetInputSize(); i++)
		{
			if((G->GetInputs())[i]->GetValue()==ZERO)
			{return DNOT;}
			else
			{
				InputValues.push_back((G->GetInputs())[i]->GetValue());
			}
		}	
		if (InputValues.size()==2)
		{	
			// input = 11, output = D;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE))
			{return D;}
		}
		else if(InputValues.size()==3)
		{
			// input = 111, output = D;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return D;}
		}
		else if(InputValues.size()==4)
		{
			// input = 1111, output D;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&(InputValues[3]==ONE))
			{return D;}
		}
	}
	else if (G->GetGateType()==OR)
	{
		vector<DType> InputValues;
		for (int i=0; i<G->GetInputSize(); i++)
		{
			if((G->GetInputs())[i]->GetValue()==ONE)
			{return D;}
			else
			{
				InputValues.push_back((G->GetInputs())[i]->GetValue());
			}
		}	
		if (InputValues.size()==2)
		{	
			// input = 00, output = D';
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO))
			{return DNOT;}
		}
		else if(InputValues.size()==3)
		{
			// input = 000, output = D';
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return DNOT;}
		}
		else if(InputValues.size()==4)
		{
			// input = 0000, output = D';
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&(InputValues[3]==ZERO))
			{return DNOT;}
		}
	}
	else if (G->GetGateType()==NAND)
	{
		vector<DType> InputValues;
		for (int i=0; i<G->GetInputSize(); i++)
		{
			if((G->GetInputs())[i]->GetValue()==ZERO)
			{return D;}
			else
			{
				InputValues.push_back((G->GetInputs())[i]->GetValue());
			}
		}	
		if (InputValues.size()==2)
		{	
			// input = 11, output = D;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE))
			{return DNOT;}
		}
		else if(InputValues.size()==3)
		{
			// input = 111, output = D;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE))
			{return DNOT;}
		}
		else if(InputValues.size()==4)
		{
			// input = 1111, output D;
			if ((InputValues[0]==ONE)&&(InputValues[1]==ONE)&&(InputValues[2]==ONE)&(InputValues[3]==ONE))
			{return DNOT;}
		}
	}
	else if (G->GetGateType()==NOR)
	{
		vector<DType> InputValues;
		for (int i=0; i<G->GetInputSize(); i++)
		{
			if((G->GetInputs())[i]->GetValue()==ONE)
			{return DNOT;}
			else
			{
				InputValues.push_back((G->GetInputs())[i]->GetValue());
			}
		}	
		if (InputValues.size()==2)
		{	
			// input = 00, output = D';
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO))
			{return D;}
		}
		else if(InputValues.size()==3)
		{
			// input = 000, output = D';
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO))
			{return D;}
		}
		else if(InputValues.size()==4)
		{
			// input = 0000, output = D';
			if ((InputValues[0]==ZERO)&&(InputValues[1]==ZERO)&&(InputValues[2]==ZERO)&(InputValues[3]==ZERO))
			{return D;}
		}
	}
	return X;
	
}
