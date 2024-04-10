#pragma once
#include <iostream>
#include <string>
#include <set>
#include "StatementInfo.h"
#include "VariableProcessing.h"
#include "../db/Database.h"


class UsesProcessing {
public:
    static void processUses(vector<StatementInfo>& statementInfo, multimap<int, int> parentChildMapping);

    static void updateUsesForAncestors(int line, string variableName,  multimap<int, int> parentChildMapping, map<int, string> lineToTypeMapping, map<int, set<string>>& usesMap);

    static vector<string> extractAssignmentVariables(const string& statement);

    static vector<string> extractPrintVariables(const string& statement);

};

