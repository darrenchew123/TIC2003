#pragma once
#include <iostream>
#include <string>
#include "../db/Database.h"

using namespace std;

class ProcessConstant {
public:
    static void processConstant(const string& constantString, int& lineCount);
};
