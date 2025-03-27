/** @file */
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <string>

#include "gates.h"

using namespace std;

void load_data(string& gatesname, unordered_map<int, gate>& gates, vector<int>& outputnodes, int& inputamount)
{
	fstream ingates;
	ingates.open(gatesname);
	if (!ingates)
	{
		gatesname.append(".txt");
		ingates.open(gatesname);
		if (!ingates)
		{
			cout << "Failed to open file " << gatesname << ". File name is wrong or the file doesnt exist\n\n\n";
			exit(EXIT_FAILURE);
		}
	}
	string hlp;
	bool wasIn = false;
	bool wasOut = false;
	while (getline(ingates, hlp))
	{
		stringstream line;
		line << hlp;
		line >> hlp;
		gate temp;
		if (hlp == "IN:")
		{
			wasIn = true;
			while (line >> hlp)
			{
				inputamount++;
				temp.name = IN;
				gates[stoi(hlp)] = temp;
 			}
			continue;
		}
		else if (hlp == "OUT:")
		{
			wasOut = true;
			while(line >> hlp)
			{
				outputnodes.push_back(stoi(hlp));
			}
			if (outputnodes.size() == 0)
			{
				cout << "ERROR: Value missing in input file.";
				exit(EXIT_FAILURE);
			}
			continue;
		}
		else if (hlp == "NEG")
		{
			gate temp;
			temp.name = NEG;
			if (line >> hlp)
			{
				temp.inx = stoi(hlp);
			}
			else
			{
				cout << "ERROR: Value missing in input file.";
				exit(EXIT_FAILURE);
			}
			if (line >> hlp)
			{
				gates[stoi(hlp)] = temp;
			}
			else
			{
				cout << "ERROR: Value missing in input file.";
				exit(EXIT_FAILURE);
			}
			continue;
		}
		else if (hlp == "AND")
		{
			temp.name = AND;
		}
		else if (hlp == "NAND")
		{
			temp.name = NAND;
		}
		else if (hlp == "OR")
		{
			temp.name = OR;
		}
		else if (hlp == "NOR")
		{
			temp.name = NOR;
		}
		else if (hlp == "XOR")
		{
			temp.name = XOR;
		}
		else if (hlp == "XNOR")
		{
			temp.name = XNOR;
		}
		else
		{
			cout << "ERROR: Value missing in input file.";
			exit(EXIT_FAILURE);
		}

		if (line >> hlp)
		{
			temp.inx = stoi(hlp);
		}
		else
		{
			cout << "ERROR: Value missing in input file.";
			exit(EXIT_FAILURE);
		}
		if (line >> hlp)
		{
			temp.iny = stoi(hlp);
		}
		else
		{
			cout << "ERROR: Value missing in input file.";
			exit(EXIT_FAILURE);
		}
		if (line >> hlp)
		{
			gates[stoi(hlp)] = temp;
		}
		else
		{
			cout << "ERROR: Value missing in input file.";
			exit(EXIT_FAILURE);
		}
		hlp = "";
	}
	if (!wasIn || !wasOut)
	{
		cout << "ERROR: No inputs or outputs provided.";
		exit(EXIT_FAILURE);
	}
	return;
}
void startevaluation(string& line, unordered_map<int, gate>& gates, ofstream& output, vector<int>& outputnodes, int& inputamount)
{
	stringstream ss;
	ss << line;
	string hlp;
	int ctr = 0;
	output << "IN: ";
	while (ss >> hlp)
	{
		stringstream temp;
		output << hlp << " ";
		string x;
		string y;
		int i = 0;
		while (hlp[i] != ':')
		{
			x.push_back(hlp[i]);
			i++;
		}
		y.push_back(hlp[i+1]);
		if (gates.count(stoi(x)) == 0)
		{
			output << "<-- ERROR: NODE MISSING DURING LOADING" << endl;
			return;
		}
		else
		{
			if(gates[stoi(x)].name == IN)
			{
				gates[stoi(x)].inx = stoi(y);
				ctr++;
			}
			else
			{
				output << "<-- ERROR: WRONG NODE DURING LOADING" << endl;
				return;
			}
		}
	}
	if (ctr != inputamount)
	{
		output << "ERROR: WRONG NODE AMMOUNT AFTER LOADING" << endl;
		return;
	}
	output << "OUT: ";
	for (auto x : outputnodes)
	{
		output << x << ":" << evaluate_gate(x, gates, output) << " ";
	}
	output << endl;
}
bool evaluate_gate(const int& gatenum,unordered_map<int, gate>& gates, ofstream& output)
{
	if (gates.count(gatenum) == 0)
	{
		output << "|ERROR: NODE " << gatenum << " MISSING: ASSUMING IT'S VALUE IS FALSE| ";
		return false;
	}
	switch (gates[gatenum].name)
	{
	case IN:
		if (gates[gatenum].inx == 1)
		{
			return true;
		}
		else if (gates[gatenum].inx == 0)
		{
			return false;
		}
		else
		{
			output << "|ERROR WHILE EVALUATING GATE " << gatenum << ": INVALID VALUE| " << gatenum;
			return false;
		}
		break;
	case AND:
		return evaluate_gate(gates[gatenum].inx, gates, output) && evaluate_gate(gates[gatenum].iny, gates, output);
		break;
	case NAND:
		return !(evaluate_gate(gates[gatenum].inx, gates, output) && evaluate_gate(gates[gatenum].iny, gates, output));
		break;
	case OR:
		return evaluate_gate(gates[gatenum].inx, gates, output) || evaluate_gate(gates[gatenum].iny, gates, output);
		break;
	case NOR:
		return !(evaluate_gate(gates[gatenum].inx, gates, output) || evaluate_gate(gates[gatenum].iny, gates, output));
		break;
	case XOR:
		return evaluate_gate(gates[gatenum].inx, gates, output) != evaluate_gate(gates[gatenum].iny, gates, output);
		break;
	case XNOR:
		return !(evaluate_gate(gates[gatenum].inx, gates, output) != evaluate_gate(gates[gatenum].iny, gates, output));
		break;
	case NEG:
		return !evaluate_gate(gates[gatenum].inx, gates, output);
		break;
	default:
		cout << "ERROR: Invalid Enum Name";
		exit(EXIT_FAILURE);
		break;
	}
	return false;
}