/** @file */
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

#include "gates.h"
using namespace std;

int main(int argc,char* argv[])
{

    string gatesname;
    string inputname;
    string outputname;

    if(argc != 7)
    {
        cout<<"-------------------------DCT INSTRUCTION MANUAL-------------------------"<<endl;
        cout<<"Please provide the following:"<<endl;
        cout<<"-u 'file name here' - input file with a circuit"<<endl;
        cout<<"-i 'file name here' - input file with inputs"<<endl;
        cout<<"-0 'file name here' - output file"<<endl;
        cout<<"Please make sure that all files are in the same folder as the program.\n\n\n";
        return 0;
    }
    else
    {
        for (int i = 1; i < 6; i += 2)
        {
            string hlp = string(argv[i]);

            if (hlp == "-u")
            {
                gatesname = string(argv[i+1]);
            }
            else if (hlp == "-i")
            {
                inputname = string(argv[i + 1]);
            }
            else if (hlp == "-o")
            {
                outputname = string(argv[i + 1]);
            }
            else
            {
                cout << "---------------------------DCT INSTRUCTION MANUAL---------------------------" << endl;
                cout << "Please provide the following:" << endl;
                cout << "-u 'file name here' - input file with a circuit" << endl;
                cout << "-i 'file name here' - input file with inputs" << endl;
                cout << "-0 'file name here' - output file" << endl;
                cout << "Please make sure that all files are in the same folder as the program code.\n\n\n";
                return 0;
            }
        }
    }
    unordered_map<int, gate> gates;
    vector<int> outputnodes;
    int inputamount = 0;
    load_data(gatesname,gates,outputnodes,inputamount);

    fstream inputs;
    inputs.open(inputname);
    if (!inputs)
    {
        inputname.append(".txt");
        inputs.open(inputname);
        if (!inputs)
        {
            cout << "Failed to open file " << inputname << ". File name is wrong or the file doesnt exist\n\n\n";
            exit(EXIT_FAILURE);
        }
    }

    ofstream output;
    if (!(outputname[outputname.length() - 1] == 't' && outputname[outputname.length() - 2] == 'x' && outputname[outputname.length() - 3] == 't' && outputname[outputname.length() - 4] == '.'))
    {
        outputname.append(".txt");
    }
    output.open(outputname);

    string hlp;
    while (getline(inputs, hlp))
    {
        startevaluation(hlp,gates,output,outputnodes,inputamount);
    }
}
