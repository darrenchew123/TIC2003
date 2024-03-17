#include "QueryProcessor.h"
#include "Database.h"
#include "utils/InfixToPostfix.h"
#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

QueryProcessor::QueryProcessor() {}

QueryProcessor::~QueryProcessor() {}

bool QueryProcessor::isT(const string& token) {
    if (token == "*")
        return 1;
    return 0;
}

string QueryProcessor::checkQuotationMarks_returnArg(int& currIdx, const vector<string>& tokens, Query& query) {

    string res = "";

    //check for isSubexpression for pattern
    if (tokens[currIdx] == "_" && tokens[currIdx + 1] == "\"") {

        currIdx += 2;

        query.pattern.isSubexpression = true;

        while (currIdx < tokens.size() && tokens[currIdx] != "\"") {
            res += tokens[currIdx];
            currIdx++;
        }
        currIdx++;
        res = InfixToPostfix::infixToPostfix(res); // convert to postfix
    }
    // tokens within quotations marks
    else if (tokens[currIdx] == "\"") {

        currIdx++;

        while (currIdx < tokens.size() && tokens[currIdx] != "\"") {
            res += tokens[currIdx];
            currIdx++;
        }
        currIdx++;
        res = InfixToPostfix::infixToPostfix(res); // convert to postfix
    }
    //single token
    else {
        res = tokens[currIdx];
        currIdx++;
    }
    return res;
}

void QueryProcessor::initSelectType(string token, Query& query) {

    query.declaredVar = token;
    query.selectType = token[0];
}

Query QueryProcessor::parser(const vector<string>& tokens) {
    Query query;

    unordered_set<string> conditionTypes{ "Follows", "Modifies", "Uses", "Parent" };
    unordered_set<string> patternTypes{ "pattern" };


    for (int i = 0; i < tokens.size(); ++i) {

        if (tokens[i] == "Select") {

            initSelectType(tokens[i + 1], query);
            i++;
        }
        else if (tokens[i] == "such" && tokens[i + 1] == "that") {

            i += 2;

            bool flag_isT = isT(tokens[i + 1]);

            if (conditionTypes.count(tokens[i]) && flag_isT) {

                query.condition.type = tokens[i];
                query.condition.isT = true;

                i += 3;
                //check for quotation marks on left/right args
                query.condition.leftArg = checkQuotationMarks_returnArg(i, tokens, query);

                i++;
                query.condition.rightArg = checkQuotationMarks_returnArg(i, tokens, query);

            }
            else if (conditionTypes.count(tokens[i])) {

                query.condition.type = tokens[i];

                i += 2;
                query.condition.leftArg = checkQuotationMarks_returnArg(i, tokens, query);
                i++;
                query.condition.rightArg = checkQuotationMarks_returnArg(i, tokens, query);
            }
        }
        //check for pattern 
        else if (patternTypes.count(tokens[i])) {

            query.pattern.patternType = tokens[i];
            query.pattern.var = tokens[i + 1];
            i += 3;
            query.pattern.patternLeftArg = checkQuotationMarks_returnArg(i, tokens, query);
            i++;
            query.pattern.patternRightArg = checkQuotationMarks_returnArg(i, tokens, query);
        }
    }

    return query;
}

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


void QueryProcessor::getParentT_Pattern_OutputParentT(string& leftArg, string& patternLeftArg, string& patternRightArg, bool isSubexpression, string& selectType, vector<string> &databaseResults) {
    vector<string> arr1;
    Database::getParentT_OutputAssign(leftArg, arr1);
    vector<string> arr2;
    Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, arr2);

    vector<string> commonStrings = QueryProcessor::findCommonStrings(arr1, arr2);

    if (!commonStrings.empty()) {
        string res = QueryProcessor::concatenateWithCommas(commonStrings);
        Database::getCombo_ParentT_Pattern_OutputStmt(res, databaseResults);

        if (!databaseResults.empty()) {
            vector<string> arr4;
            Database::getXTypeOfParents_OutputStmt(selectType, arr4);
            databaseResults = QueryProcessor::findCommonStrings(databaseResults, arr4);
        }
    }
}

void QueryProcessor::getParentT_Pattern_OutputAssign(string& leftArg, const string& patternLeftArg, string& patternRightArg, bool isSubexpression, vector<string>& databaseResults) {
    vector<string> arr1;
    Database::getParentT_OutputAssign(leftArg, arr1);
    vector<string> arr2;
    Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, arr2);

    vector<string> commonStrings = QueryProcessor::findCommonStrings(arr1, arr2);

    databaseResults = commonStrings; 
}

void QueryProcessor::getModifies_Pattern_OutputProcedure(string& rightArg, string& patternLeftArg, string& patternRightArg, bool isSubexpression, vector<string>& databaseResults) {
    vector<string> arr1;
    Database::getModifies_OutputStmt(rightArg, arr1);
    vector<string> arr2;
    Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, arr2);

    vector<string> commonStrings = QueryProcessor::findCommonStrings(arr1, arr2);

    if (!commonStrings.empty()) {
        string res = QueryProcessor::concatenateWithCommas(commonStrings);
        Database::getCombo_Modifies_Pattern_OutputProcedure(res, databaseResults);
    }
}

void QueryProcessor::getModifies_Pattern_OutputAssign(string& patternRightArg, bool isSubexpression, vector<string>& databaseResults) {
    vector<string> arr1;
    Database::getPattern_OutputStmt("_", patternRightArg, isSubexpression, arr1);

    if (!arr1.empty()) {
        string res = QueryProcessor::concatenateWithCommas(arr1);
        Database::getCombo_Modifies_Pattern_OutputAssign(res, databaseResults);
    }
}

void QueryProcessor::getModifies_Pattern_OutputVar(string& patternRightArg, bool isSubexpression, vector<string>& databaseResults) {
    vector<string> arr1;
    Database::getPattern_OutputStmt("_", patternRightArg, isSubexpression, arr1);

    if (!arr1.empty()) {
        string res = QueryProcessor::concatenateWithCommas(arr1);
        Database::getCombo_Modifies_Pattern_OutputVar(res, databaseResults);
    }
}

void QueryProcessor::getModifies_OutputParents(string& rightArg, string& selectType, vector<string>& databaseResults) {
    //Retrieve modification statements
    vector<string> LHSLines;
    Database::getModifies_OutputStmt(rightArg, LHSLines);

    //Concatenate lines if not empty
    string childrenLines;
    if (!LHSLines.empty()) {
        childrenLines = QueryProcessor::concatenateWithCommas(LHSLines);
    }

    //Retrieve parent lines based on children lines
    vector<string> ParentLinesArr;
    Database::getCombo_ParentT_Pattern_OutputStmt(childrenLines, ParentLinesArr);

    //Process parent lines and update databaseResults
    if (!ParentLinesArr.empty()) {
        string ParentLines = QueryProcessor::concatenateWithCommas(ParentLinesArr);
        Database::getModifies_OutputParents(selectType, ParentLines, databaseResults);
    }
}

//Evalutate query to get result from DB
void QueryProcessor::evaluate(string query, vector<string>& output) {
    output.clear();

    vector<string> databaseResults;

    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(query, tokens);

    Query queryToExecute = parser(tokens); 

    /* output.push_back(queryToExecute.selectType);
     output.push_back(queryToExecute.declaredVar);
     output.push_back(queryToExecute.condition.type);
     output.push_back(queryToExecute.condition.leftArg);
     output.push_back(queryToExecute.condition.rightArg);
     output.push_back(queryToExecute.pattern.patternType);
     output.push_back(queryToExecute.pattern.var);
     output.push_back(queryToExecute.pattern.patternLeftArg);
     output.push_back(queryToExecute.pattern.patternRightArg);*/

    string selectType = queryToExecute.selectType;
    string declaredVar = queryToExecute.declaredVar;
    string conditionType = queryToExecute.condition.type;
    bool isT = queryToExecute.condition.isT;
    string leftArg = queryToExecute.condition.leftArg;
    string rightArg = queryToExecute.condition.rightArg;

    string patternType = queryToExecute.pattern.patternType;
    string patternLeftArg = queryToExecute.pattern.patternLeftArg;
    string patternRightArg = queryToExecute.pattern.patternRightArg;
    bool isSubexpression = queryToExecute.pattern.isSubexpression;

    //check for combo
    if (conditionType != "" && patternType != "") {

        if ((selectType == "w" || selectType == "i") && conditionType == "Parent" && isT && patternType == "pattern") {

            QueryProcessor::getParentT_Pattern_OutputParentT(leftArg, patternLeftArg, patternRightArg, isSubexpression, selectType, databaseResults);   
        }
        else if (selectType == "a" && conditionType == "Parent" && isT && patternType == "pattern") {

            QueryProcessor::getParentT_Pattern_OutputAssign(leftArg, patternLeftArg, patternRightArg, isSubexpression, databaseResults);
        }
        else if (selectType == "p" && conditionType == "Modifies" && patternType == "pattern") {

            QueryProcessor::getModifies_Pattern_OutputProcedure(rightArg,patternLeftArg,patternRightArg,isSubexpression,databaseResults);
        }
        // Select a such that Modifies (a, v) pattern a (v, _"n"_)
        else if (selectType == "a" && conditionType == "Modifies" && patternType == "pattern" && rightArg == patternLeftArg) {
    
            QueryProcessor::getModifies_Pattern_OutputAssign(patternRightArg, isSubexpression, databaseResults);
        }
        // Select v such that Modifies (a, v) pattern a1 (v, _"n"_)
        else if (selectType == "v" && conditionType == "Modifies" && patternType == "pattern" && rightArg == patternLeftArg) {

            QueryProcessor::getModifies_Pattern_OutputVar(patternRightArg, isSubexpression, databaseResults);
        }
    }

    else {
        if (selectType == "p") {

            if (conditionType == "Modifies") {
                Database::getModifies_OutputProcedures(rightArg, databaseResults);
            }
            else if (declaredVar == "pr") {
                Database::getStatementType(selectType, databaseResults);
            }
            else
                Database::getProcedures(databaseResults);
        }
        else if (selectType == "v") {

            if (conditionType == "Modifies") {
                Database::getModifies_OutputVar(leftArg, databaseResults);
            }
            else
                Database::getVariables(databaseResults);
        }
        else if (selectType == "c") {
            Database::getConstants(databaseResults);
        }


        else if (selectType == "s") {
            if (conditionType == "Parent") {
                if (isT) {
                    Database::getParentT_OutputStmt(leftArg, databaseResults);
                }
                else {
                    Database::getParent_OutputStmt(rightArg, databaseResults);
                }
            }
            else {
                Database::getStatements(databaseResults);
            }
        }

        else if (selectType == "a") {
            if (conditionType == "Parent") {
                if (isT) {
                    Database::getParentT_OutputAssign(leftArg, databaseResults);
                }
            }
            else if (patternType == "pattern") {
                Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, databaseResults);
            }
            else
                Database::getStatementType(selectType, databaseResults);
        }
        else if (selectType == "r") {
            Database::getStatementType(selectType, databaseResults);
        }
        else if (selectType == "w" || selectType == "i") {
            if (conditionType == "Modifies") {
                QueryProcessor::getModifies_OutputParents(rightArg, selectType, databaseResults);
            }
        }
    }
    for (string res : databaseResults)
        output.push_back(res);

}

