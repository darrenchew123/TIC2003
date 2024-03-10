#include "InfixToPostfix.h"
#include <stack>
#include <string>

bool InfixToPostfix::isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

bool InfixToPostfix::isOperand(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

int InfixToPostfix::getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    return 0;
}

string InfixToPostfix::infixToPostfix(const string& infix) {
    string postfix;
    stack<char> opStack;

    for (char c : infix) {
        if (isOperand(c)) {
            postfix += c;
        } else if (c == '(') {
            opStack.push(c);
        } else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top();
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop(); // Pop the '('
        } else if (isOperator(c)) {
            while (!opStack.empty() && getPrecedence(opStack.top()) >= getPrecedence(c) && opStack.top() != '(') {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.push(c);
        }
    }

    while (!opStack.empty()) {
        if (opStack.top() != '(') {
            postfix += opStack.top();
        }
        opStack.pop();
    }
    return postfix;
}
