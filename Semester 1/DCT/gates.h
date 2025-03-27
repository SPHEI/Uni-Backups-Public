/** @file */
#include <iostream>
#include <unordered_map>
using namespace std;

#ifndef gates_h
#define gates_h
enum gatename{IN,AND,NAND,OR,NOR,XOR,XNOR,NEG};
struct gate
{
    int inx;
    int iny;
    gatename name;
};
/** The function loads data from files.
@param gatesname Name of the input file with input gates.
@param gates Unordered map which holds gates and inputs.
@param outputnodes Vector which holds output nodes.
@param inputamount Counter holding the number of input nodes.
@author Jakub Knapik
*/
void load_data(string& gatesname, unordered_map<int, gate>& gates, vector<int>& outputnodes, int& inputamount);
/** The function starts the evaluation of the output nodes, and outputs the result.
@param line Values for input nodes.
@param gates Unordered map which holds gates and inputs.
@param output Output file.
@param outputnodes Vector which holds output nodes.
@param inputamount Counter holding the number of input nodes.
@author Jakub Knapik
*/
void startevaluation(string& line, unordered_map<int, gate>& gates,ofstream& output, vector<int>& outputnodes, int& inputamount);
/** The function evaluates logic gates in order to find the value of a given node.
@param gatenum Number of the node that is being evaluated.
@param gates Unordered map which holds gates and inputs.
@param output Output file.
@return The function returns the value of the node.
@author Jakub Knapik
*/
bool evaluate_gate(const int& gatenum,unordered_map<int, gate>& gates, ofstream& output);

#endif // GATES_H
