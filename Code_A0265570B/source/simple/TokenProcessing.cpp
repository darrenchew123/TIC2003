#include "TokenProcessing.h"

bool TokenProcessing::skipTokenCheck(const string& token, int& i, const vector<string>& tokens) {
    return token == "{" || token == ";" || token == "+" || token == "-" || token == "*" ||
           token == "/" || token == "then" ||
           (token == "\n" && tokens.at(i-1) == "\n") ||
           (token == "\n" && tokens.at(i-1) == "{" && tokens.at(i-2) == "else") ||
           (token == "\n" && tokens.at(i-1) == "}");
}

// Process token to respective calls
void TokenProcessing::delegateTokenProcessing(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, stack<bool>& expressionStack, vector<StatementInfo>& statementInfo, stack<int>& ifStack, bool& pendingParentPush) {
    cout << "Delegating token: " << token << ", lineCount: " << lineCount << endl;
    if (token == "read" || token == "print" || token == "=") {
        if (expressionStack.empty() || !expressionStack.top()) {
            StatementProcessing::processReadPrintAssignment(token, statementTypes);
        }
    } else if (SyntaxValidator::isInteger(token)) {
        ProcessConstant::processConstant(token, lineCount);
    } else if (token == "\n") {
        StatementProcessing::processStatement(procedureName, token, i, lineCount, tokens, statementTypes, parentStack, statementInfo,pendingParentPush);
        lineCount++;
    } else if (token == "if" || token == "else" || token == "while" || token == "}") {
        ControlFlow::processControlFlow(token, statementTypes, parentStack, lineCount, ifStack,pendingParentPush);
    } else {
        VariableProcessing::processVariable(token, lineCount);
    }
}
