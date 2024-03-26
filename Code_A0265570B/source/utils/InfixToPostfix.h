#pragma once
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

using namespace std;

class InfixToPostfix {
private:
    static bool isOperator(char c);
    static bool isOperand(char c);
    static int getPrecedence(char op);

public:
    static string infixToPostfix(const std::string& infix);

};


