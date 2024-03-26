#pragma once
#include <sstream>
#include <vector>
#include<iostream>
#include<stack>
#include "../db/Database.h"
#include  "StatementInfo.h"

using namespace std;

class StatementProcessing {
public:
    static void processStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, vector<StatementInfo> &statementInfo, bool& pendingParentPush );

    static void processReadPrintAssignment(const string& token, stack<string>& statementTypes);

};

