#pragma once

#include <string>
#include <vector>
#include "Database.h"
#include "Tokenizer.h"


using namespace std;

struct Query {

    string selectType;

    struct Condition {

        string type;
        string leftArg;
        string rightArg;
        bool isT = false;
    } condition;

    struct Pattern {

        string var;
        string patternLeftArg;
        string patternRightArg;

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

    bool QueryProcessor::isT(const string &token);

    string QueryProcessor::checkQuotationMarks_returnArg(int &currIdx, const vector<string> &tokens);
};