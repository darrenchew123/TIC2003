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
public:
    void process(string& process);

    void processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens);

    void processFactor(const string& procedureName, const string& varName, const string& factor, int& lineCount);

    void processOtherStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens);

    void processAssignmentStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens);

    void processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens);

    };