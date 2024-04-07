#pragma once
#include <iostream>
#include <string>
#include <set>
#include "StatementInfo.h"
#include "VariableProcessing.h"
#include "../db/Database.h"

class CallsProcessing {
public:
    static void processCalls(vector<StatementInfo>& statementInfo);
    static string extractCalleeName(const string& statement);
    static void insertTransientCalls(const map<string, vector<string>>& directCalls);
    static void computeTransientForCaller(const string& caller, const map<string, vector<string>>& directCalls, map<string, set<string>>& transientCalls, set<string> visited = {});

};
