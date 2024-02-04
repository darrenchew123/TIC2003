#pragma once

#include <string>
#include <vector>
#include "Database.h"
#include "Tokenizer.h"
using namespace std;


class QueryParser {
private:
    static bool typeValidator(const string &token);

public:

    static void processObjects(const vector<string>& tokens, unordered_map<string, string> &declaredObjects);

    static void processSelect(const vector<string>& tokens, unordered_map<string, string> declaredObjects, vector<pair<string, string>>& selectObjects);

    static void parser(const vector<string>& tokens, vector<pair<string, string>>& selectObjects);
};




