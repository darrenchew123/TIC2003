#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "StatementInfo.h"
#include "../utils/SyntaxValidator.h"
#include "ExpressionProcessing.h"
#include "TokenProcessing.h"
#include "../db/Database.h"

using namespace std;

class ProcedureProcessing {
public:
    static void processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens);

    static void processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, stack<bool>& expressionStack, vector<StatementInfo>& statementInfo, stack<int>& ifStack, bool& pendingParentPush);

};
