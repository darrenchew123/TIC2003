#pragma once
#include <iostream>
#include <string>
#include<set>
#include "StatementInfo.h"
#include "VariableProcessing.h"
#include "../db/Database.h"


class ModifiesProcessing {
public:
    static void processModifies(vector<StatementInfo>& statementInfo, multimap<int,int> parentChildMapping);

    static string extractModifiesVariable(const string& statement, const string& statementType);

    static void updateModifiesForAncestors(int line, string& variableName, multimap<int,int> parentChildMapping, map<int, set<string>>& modifiesMap);

};

