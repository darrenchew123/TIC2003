#pragma once
#include <iostream>
#include <string>
#include "StatementInfo.h"
#include "VariableProcessing.h"
#include "../db/Database.h"

class CallsProcessing {
public:
    static void processCalls(vector<StatementInfo>& statementInfo);
    static string extractCalleeName(const string& statement);

};
