#pragma once
#include <iostream>
#include <string>
#include <stack>

using namespace std;

class ControlFlow {

public:
    static void processControlFlow(const string& token, stack<string>& statementTypes, stack<int>& parentStack, int& lineCount, stack<int>& ifStack, bool& pendingParentPush);
};


