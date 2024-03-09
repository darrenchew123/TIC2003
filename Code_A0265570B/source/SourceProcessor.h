#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include<iostream>
#include<stack>
#include <algorithm>
#include <regex>
#include "Database.h"
#include "Tokenizer.h"
#include "utils/InfixToPostfix.h"


using namespace std;

class SourceProcessor {
private:
    struct StatementInfo {
        int lineCount;
        std::string statementContent;
        std::string statementType;
    };
    bool isInteger(const string& intString);

    void processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens);

    void processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, stack<bool>& expressionStack, vector<StatementInfo> &statementInfo, stack<int>& ifStack);

    void processVariable(const string& varName, const int& lineCount);

    void processStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, vector<StatementInfo> &statementInfo);

    void processConstant(const string& constantString, int& lineCount);

    void processReadPrintAssignment(const string& token, stack<string>& statementTypes);

    void processControlFlow(const string& token, stack<string>& statementTypes, stack<int>& parentStack, int& lineCount, stack<int>& ifStack);

    void processExpression(std::vector<StatementInfo> &statementInfo);

    string extractVariableName(const std::string& statement, const std::string& statementType);

    void processModifies(std::vector<StatementInfo>& statementInfo);

    bool checkName(string token);

public:
    void process(string& process);

};