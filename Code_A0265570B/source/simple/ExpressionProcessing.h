#pragma once
#include <iostream>
#include <string>
#include <stack>
#include "StatementInfo.h"
#include "../utils/InfixToPostfix.h"
#include "../utils/StringUtil.h"
#include "../db/Database.h"

using namespace std;

class ExpressionProcessing {
public:
    static void handleExpressionStack(const string& token, stack<bool>& expressionStack);

    static void processExpression(vector<StatementInfo> &statementInfo);

};

