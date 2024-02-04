#pragma once

#include <string>
#include <vector>
#include "Database.h"

using namespace std;

class QueryProcessor {
public:
	QueryProcessor();

    ~QueryProcessor();

    bool typeValidator(const string &token);

    void processObjects(const vector<string>& tokens, unordered_map<string, string> &declaredObjects);

    void processSelect(const vector<string>& tokens, unordered_map<string, string> declaredObjects, vector<pair<string, string>>& selectObjects);

    void parser(const vector<string>& tokens, vector<pair<string, string>>& selectObjects);

	void evaluate(string query, vector<string>& results);
};