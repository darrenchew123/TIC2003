#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include<iostream>
#include<stack>
#include "Database.h"
#include "Tokenizer.h"

using namespace std;

class SourceProcessor {
private:
    bool isInteger(const string& intString);

    void processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens);

    void processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes);

    void processVariable(const string& varName, const int& lineCount);

    void processStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes);

    void processConstant(const string& constantString, int& lineCount);

    void processReadPrintAssignment(const string& token, stack<string>& statementTypes);

public:
    void process(string& process);

};