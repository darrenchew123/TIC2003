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
#include "HandleMultipleConditions.h"

class HandleMultipleSelect {
public:
    static void processMultiSelectQuery(string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute, bool isMultipleCond);

    static void postProcessNoConditions(vector<vector<string>>  & allResults, vector<string> &combinedResults);

    static void postProcessConditions(vector<vector<string>>  & allResults, vector<string> &combinedResults, Query queryToExecute);

    static bool checkRelationship(const std::string& relationshipType, const std::string& entity1, const std::string& entity2, const Query& query);


};


