#include "QueryParser.h"
#include "Query.h"

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <sstream>


bool QueryParser::isT(const string& token) {
    if (token == "*")
        return 1;
    return 0;
}

string QueryParser::checkQuotationMarks_returnArg(int& currIdx, const vector<string>& tokens, Query& query, Pattern& pattern) {

    string res = "";

    //check for isSubexpression for pattern
    if (tokens[currIdx] == "_" && tokens[currIdx + 1] == "\"") {

        currIdx += 2;


        pattern.isSubexpression = true;

        while (currIdx < tokens.size() && tokens[currIdx] != "\"") {
            res += tokens[currIdx];
            currIdx++;
        }
        currIdx++;
        res = InfixToPostfix::infixToPostfix(res);
    }
    // tokens within quotations marks
    else if (tokens[currIdx] == "\"") {

        currIdx++;

        while (currIdx < tokens.size() && tokens[currIdx] != "\"") {
            res += tokens[currIdx];
            currIdx++;
        }
        currIdx++;
        res = InfixToPostfix::infixToPostfix(res);
    }
    //single token, to be checked against delcared objects
    else {
        res = tokens[currIdx];
        currIdx++;
    }
    return res;
}

//to store variable/s declared eg. assign a1 , oioioi ; while w1 , w2 ;
vector<string> QueryParser::Utility_appendObject(const vector<string>& tokens, string currToken, int& i, string end, string delimiter) {

    vector<string> objects;

    while (currToken != end) {
        if (currToken == delimiter) {
            i++;
            currToken = tokens[i];
        }
        else {
            objects.push_back(currToken);
            i++;
            currToken = tokens[i];
        }
    }
    return objects;
}

Query QueryParser::parser(const vector<string>& tokens) {
    Query query;

    unordered_set<string> dataType{ "assign","while","stmt", "variable", "print", "read", "if", "constant", "procedure", "if" };
    unordered_set<string> conditionTypes{ "Follows", "Modifies", "Uses", "Parent" , "Next", "calls" };
    unordered_set<string> patternTypes{ "pattern" };


    for (int i = 0; i < tokens.size(); ++i) {

        //variables declaration stmt s, s1; assign a, a1; while w; if ifs; variable v, v1; procedure p; constant c; read re; print pn;
        if (dataType.count(tokens[i])) {
            string varType = tokens[i];
            i++;
            vector<string> varNames = Utility_appendObject(tokens, tokens[i], i, ";", ",");
            for (auto varName : varNames) {
                //query.variables.push_back({varType, varName});
                query.declaredVariables.insert({ varName, varType });
            }
        }
        else if (tokens[i] == "Select") {

            i++;
            if (tokens[i] == "<") {
                i++;
                vector<string> vars = Utility_appendObject(tokens, tokens[i], i, ">", ",");
                for (string var : vars) {
                    if (query.declaredVariables.count(var))
                        query.multiSelectType.insert(query.declaredVariables[var]);
                }
            }
            else {
                if (query.declaredVariables.count(tokens[i])) {
                    query.selectType = query.declaredVariables[tokens[i]];
                    query.selectVar = tokens[i];
                }
            }

        }
        //store condition/s
        else if (tokens[i] == "such" && tokens[i + 1] == "that") {

            i += 2;

            Condition condition;
            Pattern dummy;
            //check if next is "*" (transitive)
            bool flag_isT = isT(tokens[i + 1]);

            if (conditionTypes.count(tokens[i]) && flag_isT) {

                condition.conditionType = tokens[i];
                condition.isT = true;

                i += 3;
                //check for quotation marks on left/right args
                condition.leftArg = checkQuotationMarks_returnArg(i, tokens, query, dummy);

                i++;
                condition.rightArg = checkQuotationMarks_returnArg(i, tokens, query, dummy);

                query.conditions.push_back(condition);
            }
            else if (conditionTypes.count(tokens[i])) {

                condition.conditionType = tokens[i];

                i += 2;
                condition.leftArg = checkQuotationMarks_returnArg(i, tokens, query, dummy);
                i++;
                condition.rightArg = checkQuotationMarks_returnArg(i, tokens, query, dummy);

                query.conditions.push_back(condition);
            }
        }
        //check for pattern 
        else if (patternTypes.count(tokens[i])) {

            Pattern pattern;

            pattern.patternType = tokens[i];
            pattern.patternVar = tokens[i + 1];
            i += 3;
            pattern.patternLeftArg = checkQuotationMarks_returnArg(i, tokens, query, pattern);
            i++;
            pattern.patternRightArg = checkQuotationMarks_returnArg(i, tokens, query, pattern);

            query.patterns.push_back(pattern);
        }
    }

    return query;
}