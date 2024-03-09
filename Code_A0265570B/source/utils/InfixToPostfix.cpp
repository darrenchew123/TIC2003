#include "InfixToPostfix.h"

bool InfixToPostfix::isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool InfixToPostfix::isOperand(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

int InfixToPostfix::getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

string InfixToPostfix::infixToPostfix(const std::string& infix) {
    std::string postfix;
    std::stack<char> opStack;

    for (char c : infix) {
        if (isOperand(c)) {
            postfix += c;
        } else if (isOperator(c)) {
            while (!opStack.empty() && getPrecedence(opStack.top()) >= getPrecedence(c)) {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.push(c);
        }
    }

    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }
    return postfix;
}