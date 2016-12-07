#include <iostream>
#include <fstream>
#include <sstream>
#include "fcntl.h"

using namespace std;

int main(int argc, char **argv)
{
	ifstream       benchmark;
    fstream        outputfile[10];
	stringstream   str;

    int d = 0;

    string filename[10];
	string temp;
    string line;
    string firstline;
    string name;
    string input[4];
    string output;


    if (argc == 1)
	{
		cout << "Error: need file name" << endl;
		return 0;
	}
	else if (argc == 2)
	{
        benchmark.open(argv[1], ios::in);
		if (!benchmark.good())
		{
			cout << "Error: no such file" << endl;
			return 0;
		}
	}
	else
	{
		cout << "Error: wrong input" << endl;
		return 0;
	}
    for (int i = 0; i < 10; i++)
    {
        str << argv[1] << '_' << i;
        str >> filename[i];
        fclose(fopen(filename[i].c_str(), "w"));
        outputfile[i].open(filename[i].c_str(), ios::out | ios::in | ios::trunc);
        str.clear();
    }
    
    while(getline(benchmark, line))
    {
        str << line;
        str >> temp;
        if (temp == "input")
        {
            for (int i = 0; i < 10; i++)
            {
                outputfile[i] << "input";
            }
            while(str >> temp)
            {
                for (int i = 0; i < 10; i++)
                {
                    outputfile[i] << " " << temp << "_" << 0;

                }
            }
            for (int i = 0; i < 10; i++)
            {
                outputfile[i] << endl;
            }
        }
        else if (temp == "output")
        {
            for (int i = 0; i < 10; i++)
            {
                outputfile[i] << "output";
            }
            while (str >> temp)
            {
                for (int i = 0; i < 10; i++)
                {
                    outputfile[i] << " " << temp << "_" << i;
                }
            }
            for (int i = 0; i < 10; i++)
            {
                outputfile[i] << endl;
            }
        }
        else if (temp == "wire")
        {
            for (int i = 0; i < 10; i++)
            {
                outputfile[i] << "wire";
            }
            while (str >> temp)
            {
                for (int i = 0; i < 10; i++)
                {
                    for (int j = 0; j <= i; j++)
                    {
                        outputfile[i] << " " << temp << "_" << j;
                    }
                }
            }
            for (int i = 0; i < 10; i++)
            {
                outputfile[i] << endl;
            }
        }
        else if (temp == "dff")
        {
            str >> name >> output >> input[0];

            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j <= i; j++)
                {
                    if (j == 0)
                    {
                        outputfile[i] << "dff " << name << "_0 " << output << "_0 " << "D" << d;
                        //outputfile[i].close();
                        //outputfile[i].open(filename[i].c_str(), ios::out | ios::in | ios::trunc);
                        //getline(outputfile[i], firstline);
                        //outputfile[i].seekp(sizeof(firstline), outputfile[i].beg);
                        //outputfile[i] << " " << "D" << d;
                        //outputfile[i].seekp(outputfile[i].end);
                        outputfile[i] << endl;
                    }
                    else
                    {
                        outputfile[i] << "dff " << name << "_" << j << " " << output << "_" << j << " " << input[0] << "_" << j-1;
                        outputfile[i] << endl;
                    }
                    
                }
            }
            d += 1;
        }
        else if (temp != "")
        {
            str >> name >> output;
            int num_in = 0;
            while (str >> input[num_in])
            {
                num_in++;
            }
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j <= i; j++)
                {
                    outputfile[i] << temp << " " << name << "_" << j << " " << output << "_" << j;
                    for (int n = 0; n < num_in; n++)
                    {
                         outputfile[i] << " " << input[n] << "_" << j;
                    }
                    outputfile[i] << endl;
                }
            }
        }
        str.clear();
        temp.clear();
    }
    benchmark.close();
    for (int i = 0; i < 10; i++)
    {
        outputfile[i].close();
    }
    return 0;
}