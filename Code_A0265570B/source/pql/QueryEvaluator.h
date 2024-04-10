#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../db/Database.h"
#include "../utils/Tokenizer.h"
#include "QueryParser.h"
#include "QueryProcessor.h"
#include "Query.h"


using namespace std;


class QueryEvaluator {

public:

    QueryEvaluator();

    ~QueryEvaluator();

    static void evaluate(string query, vector<string>& output);

    static void process_multiSelect(string selectVar, string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute);
    static void handleParentCondition(string selectVar, string selectType, string leftArg,  string rightArg, vector<string>& databaseResults, Query& queryToExecute, bool isT);

    static void processSingleSelectMultiCond(string selectVar, string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute);

    static void processSimpleQuery(string selectVar, string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute);

};