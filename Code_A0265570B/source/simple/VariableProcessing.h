#pragma once
#include <sstream>
#include <vector>
#include <iostream>
#include <stack>
#include "../db/Database.h"
#include "StatementProcessing.h"
#include "../utils/SyntaxValidator.h"

using namespace std;

class VariableProcessing {
public:
    static void processVariable(const string& varName, const int& lineCount);

};

