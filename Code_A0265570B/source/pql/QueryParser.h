#pragma once

#include <string>
#include <vector>
#include "Query.h"
#include "../db/Database.h"
#include "../utils/Tokenizer.h"
#include "../utils/InfixToPostfix.h"
#include "../pql/QueryProcessor.h"
#include <unordered_set>
using namespace std;


class QueryParser {
public:

    static bool isT(const string& token);

    static string checkQuotationMarks_returnArg(int& currIdx, const vector<string>& tokens, Query& query, Pattern& pattern);

    static Query parser(const vector<string>& tokens);

    //can put in utility
    static vector<string> Utility_appendObject(const vector<string>& tokens, string currToken, int& i, string end, string delimiter);

};




