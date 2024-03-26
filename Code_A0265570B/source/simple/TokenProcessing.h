#pragma once
#include <sstream>
#include <vector>
#include <iostream>
#include <stack>
#include "../db/Database.h"
#include "StatementInfo.h"
#include "../utils/SyntaxValidator.h"
#include "StatementProcessing.h"
#include "ProcessConstant.h"
#include "ControlFlow.h"
#include "VariableProcessing.h"
#include "../utils/SyntaxValidator.h"

using namespace std;

class TokenProcessing {
public:
    static bool skipTokenCheck(const string& token, int& i, const vector<string>& tokens);

    static void delegateTokenProcessing(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, stack<bool>& expressionStack, vector<StatementInfo>& statementInfo, stack<int>& ifStack, bool& pendingParentPush);
};

