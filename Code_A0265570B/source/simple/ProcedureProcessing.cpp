#include "ProcedureProcessing.h"

// Process procedure
void ProcedureProcessing::processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens) {
    cout << "Entering processProcedure with token: " << tokens[i] << endl;
    inProcedure = true;
    procedureName = tokens[++i]; // Move to the next token to get the procedure name

    if (!SyntaxValidator::checkName(procedureName)) {
        cerr << "Procedure name does not adhere to naming convention: " << procedureName << endl;
        throw runtime_error("Procedure name error");
    }

    Database::insertProcedure(procedureName);
}

// Process SIMPLE code inside the procedure
void ProcedureProcessing::processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, stack<bool>& expressionStack, vector<StatementInfo>& statementInfo, stack<int>& ifStack, bool& pendingParentPush) {
    if (TokenProcessing::skipTokenCheck(token, i, tokens)) {
        return;
    }

    ExpressionProcessing::handleExpressionStack(token, expressionStack);
    TokenProcessing::delegateTokenProcessing(token, procedureName, i, lineCount, tokens, statementTypes, parentStack, expressionStack, statementInfo, ifStack,pendingParentPush);
}