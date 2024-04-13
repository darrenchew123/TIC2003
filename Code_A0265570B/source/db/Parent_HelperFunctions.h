#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "sqlite3.h"
#include "../pql/Query.h"
#include "Database.h"

class Parent_HelperFunctions {
public:

    static void handleWildcardLHS(string &getParentSQL, const string& selectVar, const string& selectType, const string& lhsSynType, const string& rhsSynType, const string& leftArg, const string& rightArg, bool isrhsSyn, vector<string>& results, vector<vector<string>>& dbResults);
    static void handleSynonymLHS(string& getParentSQL, const string& selectVar, const string& selectType, const string& lhsSynType, const string& rhsSynType, const string& rightArg, const string& leftArg, bool isrhsSyn, vector<string>& results, vector<vector<string>>& dbResults);
    static void handleLineNumberLHS(string& getParentSQL, const string& selectVar, const string& selectType, const string& lhsSynType, const string& rhsSynType, const string& leftArg, const string& rightArg, bool isrhsSyn, vector<string>& results, vector<vector<string>>& dbResults);

};