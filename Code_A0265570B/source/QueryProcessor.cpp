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
    // tokens withing quotations marks
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

vector<string> QueryProcessor::findCommonStrings(const vector<string>& arr1, const vector<string>& arr2) {
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
            vector<string> arr1;
            Database::getParentT_OutputStmt(leftArg, arr1);
            vector<string> arr2;
            Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, arr2);
            
            vector<string> commonStrings = QueryProcessor::findCommonStrings(arr1, arr2);

            if (!commonStrings.empty()) {
                string res = QueryProcessor::concatenateWithCommas(commonStrings);
                Database::getCombo_ParentT_Pattern_OutputStmt(res, databaseResults);
            }
        }
        else if (selectType == "a" && conditionType == "Parent" && isT && patternType == "pattern") {
            vector<string> arr1;
            Database::getParentT_OutputStmt(leftArg, arr1);
            vector<string> arr2;
            Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, arr2);

            vector<string> commonStrings = QueryProcessor::findCommonStrings(arr1, arr2);

            databaseResults = commonStrings; // return statementLines with assignments in a while loop
        }
        else if (selectType == "p" && conditionType == "Modifies" && patternType == "pattern") {

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
    }

    else {
        if (selectType == "p") {

            if (conditionType == "Modifies") {
                Database::getModifies_OutputProcedures(rightArg, databaseResults);
            }
            else
                Database::getProcedures(databaseResults);
        }
        else if (selectType == "v") {

            if (conditionType == "Modifies") {
                Database::getModifies_OutputVar(leftArg, databaseResults);
            }
            // Select v1 such that Uses (14, v1) 
            /*else if (conditionType == "Uses") {
                Database::getUses_OutputVar(queryToExecute.condition.leftArg, databaseResults);
            }*/
            else
                Database::getVariables(databaseResults);
        }
        else if (selectType == "c") {
            Database::getConstants(databaseResults);
        }


        else if (selectType == "s") {

            // iteration 2: stmt s; Select s such that Follows* (6, s)
            if (conditionType == "Follows") {
                /*if (isT) {
                    Database::FollowsT(leftArg, databaseResults);
                }*/
            }
            else if (conditionType == "Parent") {
                if (isT) {

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
                    Database::getParentT_OutputStmt(leftArg, databaseResults);
                }
            }
            /*else if (conditionType == "Uses") {
                Database::getUses_OutputStmt(rightArg, databaseResults);
            }*/
            else if (patternType == "pattern") {
                Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, databaseResults);
            }
            else 
                Database::getStatementType(selectType, databaseResults);
        }
        else if (selectType == "p") {
            Database::getStatementType(selectType, databaseResults);
        }
        else if (selectType == "r") {
            Database::getStatementType(selectType, databaseResults);
        }
    }
    for (string res : databaseResults)
        output.push_back(res);

}

