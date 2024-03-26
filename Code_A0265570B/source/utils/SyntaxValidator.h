#pragma once
#include <string>
#include <regex>

using namespace std;


class SyntaxValidator {
public:
    static bool checkName(const string& token);
    static bool isInteger(const string& token);
};


