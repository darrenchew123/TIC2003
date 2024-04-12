#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../db/Database.h"
#include "../utils/Tokenizer.h"
#include "QueryParser.h"
#include "QueryProcessor.h"
#include "Query.h"
#include "HandleSimpleQueries.h"

class HandleMultipleConditions{
public:
    static void processSingleSelectMultiCond(string selectVar, string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute) ;
    static void intersectResults(vector<string>& results, vector<string> curr) ;
    static void processCondition(Condition condition, string selectVar, string selectType, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& curr, Query queryToExecute) ;
    static void processPattern(Pattern pattern, string selectVar, string selectType, bool isSubexpression, vector<string>& curr, Query queryToExecute) ;
};
