#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include<iostream>
#include "Database.h"
#include "Tokenizer.h"

using namespace std;

class SourceProcessor {
private:
    void processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens);

    void processConstantAssignment(const string& procedureName, const string& varName, const string& constantString, int& lineCount);

    void processOtherStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens);

    void processAssignmentStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens);

    void processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens);

public:
    void process(string& process);

    };