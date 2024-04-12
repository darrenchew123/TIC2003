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
#include "HandleMultipleSelect.h"



using namespace std;


class QueryEvaluator {

public:

    QueryEvaluator();

    ~QueryEvaluator();

    static void evaluate(string query, vector<string>& output);
    static void extractConditions(const Query& queryToExecute, string& conditionType, bool& isT, string& leftArg, string& rightArg);
    static void extractPatterns(const Query& queryToExecute, string& patternType, string& patternLeftArg, string& patternRightArg, bool& isSubexpression);\
    static void determineQueryComplexity(const Query& queryToExecute, bool& isMultipleCond, bool& isMultiSelect);
    static void debugPrintQuery(const Query& queryToExecute);


    };
