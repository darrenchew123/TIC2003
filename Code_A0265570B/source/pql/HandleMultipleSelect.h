#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "../db/Database.h"
#include "../utils/Tokenizer.h"
#include "QueryParser.h"
#include "QueryProcessor.h"
#include "Query.h"
#include "HandleSimpleQueries.h"
#include "HandleMultipleConditions.h"

class HandleMultipleSelect {
public:
    static void processMultiSelectQuery(string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute, bool isMultipleCond);

    static void postProcessNoConditions(vector<vector<string>>  & allResults, vector<string> &combinedResults);

    static void postProcessConditions(unordered_map<string, pair<int, vector<string>>> resultsMap, vector<string>& combinedResults, const Query& queryToExecute);

    static bool checkRelationship(string relationshipType, string entity1, string entity2, bool isT, Query queryToExecute, Condition condition);

    static void generateCombinations(unordered_map<string, pair<int, vector<string>>> resultsMap, vector<string>& entityOrder, int index, vector<string>& current, vector<vector<string>>& allCombinations);

};


