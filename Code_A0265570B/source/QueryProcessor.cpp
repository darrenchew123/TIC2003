#include "QueryProcessor.h"
#include "Database.h"
#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

QueryProcessor::QueryProcessor() {}

QueryProcessor::~QueryProcessor() {}

bool QueryProcessor::isT(const string &token) {
    if (token == "*")
        return 1;
    return 0;
}

string QueryProcessor::checkQuotationMarks_returnArg(int &currIdx, const vector<string> &tokens) {
    
    string res = "";

    if (tokens[currIdx] == "\"") {
        
        currIdx++;

        while (currIdx < tokens.size() && tokens[currIdx] != "\"") {
            res += tokens[currIdx];
            currIdx++;
        }
        currIdx++;
    }
    else {
        res = tokens[currIdx];
        currIdx++;
    }
    return res;
}

Query QueryProcessor::parser(const vector<string>& tokens) {
    Query query;

    unordered_set<string> conditionTypes{ "Follows", "Modifies", "Uses", "Parent" };
    unordered_set<string> patternTypes{ "pattern" };


    for (int i = 0; i < tokens.size(); ++i) {

        if (tokens[i] == "Select") {

            query.selectType = tokens[i + 1]; // Assuming the next token is the select type
            i++; // Move past the select type
        }
        else if (tokens[i] == "such" && tokens[i + 1] == "that") {
            // Parse conditions like "Follows*", "Modifies", "Parent*", "Uses"
            i += 2;
            // cout << tokens[i+1];
            //check if next token is "*"
            bool flag_isT = isT(tokens[i + 1]);
            // cout << flag_isT;
            if (conditionTypes.count(tokens[i]) && flag_isT) {

                query.condition.type = tokens[i];
                query.condition.isT = true;

                i += 3;
                //check for quotation marks on left/right args
                query.condition.leftArg = checkQuotationMarks_returnArg(i, tokens);

                i++;
                query.condition.rightArg = checkQuotationMarks_returnArg(i, tokens);

            }
            else if (conditionTypes.count(tokens[i])) {

                query.condition.type = tokens[i];

                i += 2;
                query.condition.leftArg = checkQuotationMarks_returnArg(i, tokens);
                i++;
                query.condition.rightArg = checkQuotationMarks_returnArg(i, tokens);
            }
        }
        //check for pattern combinations
        else if (patternTypes.count(tokens[i])) {

            query.pattern.var = tokens[i + 1];
            i += 3;
            query.pattern.patternLeftArg = checkQuotationMarks_returnArg(i, tokens);
            i++;
            query.pattern.patternRightArg = checkQuotationMarks_returnArg(i, tokens);
        }
    }

    return query;
}

//Evalutate query to get result from DB
void QueryProcessor::evaluate(string query, vector<string>& output) {
	output.clear();

    vector<string> databaseResults;

    Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(query, tokens);

    Query queryToExecute = parser(tokens);

    output.push_back(queryToExecute.selectType);
    output.push_back(queryToExecute.condition.type);
    output.push_back(queryToExecute.condition.leftArg);
    output.push_back(queryToExecute.condition.rightArg);
    output.push_back(queryToExecute.pattern.var);
    output.push_back(queryToExecute.pattern.patternLeftArg);
    output.push_back(queryToExecute.pattern.patternRightArg);

    /*if (queryToExecute.selectType == "p") {
        Database::getProcedures(databaseResults);
    }
    else if (queryToExecute.selectType == "v") {
        Database::getProcedures(databaseResults);
    }
    else if (queryToExecute.selectType == "c") {
        Database::getProcedures(databaseResults);
    }
    else if (queryToExecute.selectType == "a") {
        Database::getProcedures(databaseResults);
    }
    else if (queryToExecute.selectType == "pr") {
        Database::getProcedures(databaseResults);
    }
    else if (queryToExecute.selectType == "rd") {
        Database::getProcedures(databaseResults);
    }
    else if (queryToExecute.selectType == "s") {
        Database::getProcedures(databaseResults);
    }
    
    for (string databaseresult : databaseResults) {
        output.push_back(databaseresult);
    }*/
}

