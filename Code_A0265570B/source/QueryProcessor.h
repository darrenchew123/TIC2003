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

    bool isT(const string& token);

    string checkQuotationMarks_returnArg(int& currIdx, const vector<string>& tokens, Query& query);

    void initSelectType(string token, Query& query);

    vector<string> findCommonStrings(vector<string>& arr1, vector<string>& arr2);

    string concatenateWithCommas(const vector<string>& commonStrings);
};