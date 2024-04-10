#include "QueryProcessor.h"

using namespace std;

QueryProcessor::QueryProcessor() {}

QueryProcessor::~QueryProcessor() {}


vector<string> QueryProcessor::findCommonStrings(vector<string>& arr1, vector<string>& arr2) {
    unordered_set<string> elementsSet(arr1.begin(), arr1.end());
    vector<string> commonStrings;

    for (const string& elem : arr2) {
        // If element is found in set, then it exists in both arrays.
        if (elementsSet.find(elem) != elementsSet.end()) {
            commonStrings.push_back(elem);
        }
    }

    return commonStrings;
}

string QueryProcessor::concatenateWithCommas(const vector<string>& commonStrings) {
    if (commonStrings.empty()) {
        return "";
    }

    string res = commonStrings[0];
    for (int i = 1; i < commonStrings.size(); i++) {
        res += ",";
        res += commonStrings[i];
    }

    return res;
}

void QueryProcessor::getModifies_Pattern_OutputProcedure(string& rightArg, string& patternLeftArg, string& patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute) {
    vector<string> arr1;
    Database::getModifies_OutputStmt(rightArg, arr1,queryToExecute);
    vector<string> arr2;
    Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, arr2,queryToExecute);

    vector<string> commonStrings = QueryProcessor::findCommonStrings(arr1, arr2);

    if (!commonStrings.empty()) {
        string res = QueryProcessor::concatenateWithCommas(commonStrings);
        Database::getCombo_Modifies_Pattern_OutputProcedure(res, databaseResults);
    }
}

void QueryProcessor::getModifies_Pattern_OutputAssign(string& patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute) {
    vector<string> arr1;
    Database::getPattern_OutputStmt("_", patternRightArg, isSubexpression, arr1,queryToExecute);

    if (!arr1.empty()) {
        string res = QueryProcessor::concatenateWithCommas(arr1);
        Database::getCombo_Modifies_Pattern_OutputAssign(res, databaseResults);
    }
}

void QueryProcessor::getModifies_Pattern_OutputVar(string& patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute) {
    vector<string> arr1;
    Database::getPattern_OutputStmt("_", patternRightArg, isSubexpression, arr1, queryToExecute);

    if (!arr1.empty()) {
        string res = QueryProcessor::concatenateWithCommas(arr1);
        Database::getCombo_Modifies_Pattern_OutputVar(res, databaseResults);
    }
}
