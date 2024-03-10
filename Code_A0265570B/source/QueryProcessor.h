#pragma once

#include <string>
#include <vector>
#include "Database.h"
#include "Tokenizer.h"


using namespace std;

struct Query {

    string selectType;
    string declaredVar;

    struct Condition {

        string type;
        string leftArg;
        string rightArg;
        bool isT = false;
    } condition;

    struct Pattern {

        string patternType;
        string var;
        string patternLeftArg;
        string patternRightArg;
        bool isSubexpression = false;

    } pattern;
};

class QueryProcessor {
public:
    QueryProcessor();

    ~QueryProcessor();

    //bool typeValidator(const string &token);

    //void processObjects(const vector<string>& tokens, unordered_map<string, string> &declaredObjects);

    //void processSelect(const vector<string>& tokens, unordered_map<string, string> declaredObjects, vector<pair<string, string>>& selectObjects);

    Query parser(const vector<string>& tokens);

    void evaluate(string query, vector<string>& results);

    bool QueryProcessor::isT(const string& token);

    string QueryProcessor::checkQuotationMarks_returnArg(int& currIdx, const vector<string>& tokens, Query& query);

    void QueryProcessor::initSelectType(string token, Query& query);

    vector<string> QueryProcessor::findCommonStrings(const vector<string>& arr1, const vector<string>& arr2);

    string QueryProcessor::concatenateWithCommas(const vector<string>& commonStrings);
};