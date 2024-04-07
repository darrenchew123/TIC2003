#include "TokenProcessing.h"

bool TokenProcessing::skipTokenCheck(const string& token, int& i, const vector<string>& tokens) {
    return token == "{" || token == ";" || token == "+" || token == "-" || token == "*" ||
           token == "/" || token == "then" || tokens.at(i-1) == "call" ||
           (token == "\n" && tokens.at(i-1) == "\n") ||
           (token == "\n" && tokens.at(i-1) == "{" && tokens.at(i-2) == "else") ||
           (token == "\n" && tokens.at(i-1) == "}") ||
           (token == "\n" && tokens.at(i-3) == "procedure");

}

// Process token to respective calls
void TokenProcessing::delegateTokenProcessing(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, stack<bool>& expressionStack, vector<StatementInfo>& statementInfo, stack<int>& ifStack, bool& pendingParentPush, multimap<int,int> &parentChildMapping) {
    cout << "Delegating token: " << token << ", lineCount: " << lineCount << endl;
    if (token == "read" || token == "print" || token == "=" || token == "call" ) {
        if (expressionStack.empty() || !expressionStack.top()) {
            StatementProcessing::processStatementStack(token, statementTypes);
        }
    } else if (SyntaxValidator::isInteger(token)) {
        ProcessConstant::processConstant(token, lineCount);
    } else if (token == "\n") {
        StatementProcessing::processStatement(procedureName, token, i, lineCount, tokens, statementTypes, parentStack, statementInfo,pendingParentPush,parentChildMapping);
        lineCount++;
    } else if (token == "if" || token == "else" || token == "while" || token == "}") {
        ControlFlow::processControlFlow(token, statementTypes, parentStack, lineCount, ifStack,pendingParentPush);
    } else {
        VariableProcessing::processVariable(token, lineCount);
    }
}
