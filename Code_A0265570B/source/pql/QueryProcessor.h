#pragma once

#include <iostream>
#include <unordered_set>
#include <string>
#include <vector>
#include "../utils/Tokenizer.h"
#include "../db/Database.h"
#include "../utils/InfixToPostfix.h"
#include "Query.h"
#include "QueryParser.h"



using namespace std;

class QueryProcessor {
public:
    QueryProcessor();

    ~QueryProcessor();


    static vector<string> findCommonStrings(vector<string>& arr1, vector<string>& arr2);

    static string concatenateWithCommas(const vector<string>& commonStrings);

    static void getModifies_Pattern_OutputProcedure(string& rightArg, string& patternLeftArg, string& patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute);

    static void getModifies_Pattern_OutputAssign(string& patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute);

    static void getModifies_Pattern_OutputVar(string& patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute);

    static void getModifies_OutputParents(string& rightArg, string& selectType, vector<string>& databaseResults,Query queryToExecute);
};