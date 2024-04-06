#pragma once
#include <iostream>
#include <string>
#include "StatementInfo.h"
#include "VariableProcessing.h"
#include "../db/Database.h"


class UsesProcessing {
public:
    static void processUses(vector<StatementInfo>& statementInfo);

    static vector<string> extractAssignmentVariables(const string& statement);

    static vector<string> extractPrintVariables(const string& statement);

};

