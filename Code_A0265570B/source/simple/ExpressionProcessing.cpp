#include "ExpressionProcessing.h"

// Process "()"
void ExpressionProcessing::handleExpressionStack(const string& token, stack<bool>& expressionStack) {
    if (token == "(") {
        expressionStack.push(true);
    } else if (token == ")" && !expressionStack.empty()) {
        expressionStack.pop();
    }
}

// Process expression to LHS RHS
void ExpressionProcessing::processExpression(vector<StatementInfo> &statementInfo) {
    for (const auto& info : statementInfo) {
        if(info.statementType != "assign") continue;
        size_t equalsPosition = info.statementContent.find('=');
        if (equalsPosition != string::npos) {
            string lhs = info.statementContent.substr(0, equalsPosition);
            string rhs = info.statementContent.substr(equalsPosition + 1);

            rhs = InfixToPostfix::infixToPostfix(rhs);

            lhs = StringUtil::trim(lhs);
            rhs = StringUtil::trim(rhs);

            Database::insertPattern(info.lineCount, lhs, rhs);
        } else {
            cerr << "Assignment operator not found in statement: " << info.statementContent << endl;
        }
    }
}