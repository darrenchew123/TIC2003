#include "ControlFlow.h"

// Process control flow for if/else/while
void ControlFlow::processControlFlow(const string& token, stack<string>& statementTypes, stack<int>& parentStack, int& lineCount, stack<int>& ifStack, bool& pendingParentPush) {
    if (token == "if" || token == "while") {
        statementTypes.push(token);
        cout << "Pushed to statementTypes: " << token << endl;
        pendingParentPush = true;
        if (token == "if") {
            ifStack.push(lineCount);
        }
    } else if (token == "else") {
        if (ifStack.empty()) {
            throw runtime_error("Syntax error: 'else' without preceding 'if'");
        }
        // Handling else; popping the ifStack indicates closing the if block
        cout << "Else push: " <<ifStack.top() <<endl;
        parentStack.push(ifStack.top());
        ifStack.pop();
    } else if (token == "}") {
        if (!parentStack.empty()) {
            cout << "Deleting parentChild parent :" << parentStack.top() << " child: " << lineCount << endl;
            parentStack.pop();
        }
    }
}