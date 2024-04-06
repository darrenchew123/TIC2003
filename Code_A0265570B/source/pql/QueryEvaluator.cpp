#include "QueryEvaluator.h"
#include "QueryParser.h"

QueryEvaluator::QueryEvaluator() {}

QueryEvaluator::~QueryEvaluator() {}


//Evalutate query to get result from DB
void QueryEvaluator::evaluate(string query, vector<string>& output) {
    output.clear();

    vector<string> databaseResults;

    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(query, tokens);

   /* for (string i : tokens)
        output.push_back(i);

    output.push_back("END");*/

    Query queryToExecute = QueryParser::parser(tokens);

    /*for (auto a : queryToExecute.declaredVariables) {
        output.push_back(a.first);
        output.push_back("type=");
        output.push_back(a.second);
    }*/

    /*for (Condition c : queryToExecute.conditions) {
        output.push_back("cond:");
        output.push_back(c.type);
        output.push_back(c.leftArg);
        output.push_back(c.rightArg);
    }

    for (Pattern a : queryToExecute.patterns) {
        output.push_back("pattern:");
        output.push_back(a.patternType);
        output.push_back(a.patternVar);
        output.push_back(a.patternLeftArg);
        output.push_back(a.patternRightArg);
    }*/

    string selectType = queryToExecute.selectType;
    string selectVar = queryToExecute.selectVar;

    string conditionType;
    bool isT = 0;
    string leftArg;
    string rightArg;

    if (!queryToExecute.conditions.empty()) {
            conditionType = queryToExecute.conditions[0].conditionType;
            isT = queryToExecute.conditions[0].isT;
            leftArg = queryToExecute.conditions[0].leftArg;
            rightArg = queryToExecute.conditions[0].rightArg;
    }

    string patternType;
    string patternLeftArg;
    string patternRightArg;
    bool isSubexpression = 0;

    if (!queryToExecute.patterns.empty()) {
        patternType = queryToExecute.patterns[0].patternType;
        patternLeftArg = queryToExecute.patterns[0].patternLeftArg;
        patternRightArg = queryToExecute.patterns[0].patternRightArg;
        isSubexpression = queryToExecute.patterns[0].isSubexpression;
    }

    //output.push_back(selectType);
    //output.push_back(conditionType);
    //output.push_back(patternType);

    //Determine if query is a combo or simple type and process
    if (!conditionType.empty() && !patternType.empty()) {
        processComboQuery(selectVar, selectType, conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
    } else {
        processSimpleQuery(selectVar, selectType, conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
    }

    output.insert(output.end(), databaseResults.begin(), databaseResults.end());
}
//
// Process combo queries
void QueryEvaluator::processComboQuery(string selectVar, string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute) {
    if ((selectType == "while" || selectType == "if") && conditionType == "Parent" && isT && patternType == "pattern") {
        QueryProcessor::getParentT_Pattern_OutputParentT(leftArg, patternLeftArg, patternRightArg, isSubexpression, selectType, databaseResults,queryToExecute);
    }
    else if (selectType == "assign" && conditionType == "Parent" && isT && patternType == "pattern") {
        QueryProcessor::getParentT_Pattern_OutputAssign(leftArg, patternLeftArg, patternRightArg, isSubexpression, databaseResults,queryToExecute);
    }
    else if (selectType == "procedure" && conditionType == "Modifies" && patternType == "pattern") {
        QueryProcessor::getModifies_Pattern_OutputProcedure(rightArg,patternLeftArg,patternRightArg,isSubexpression,databaseResults,queryToExecute);
    }
        // Select a such that Modifies (a, v) pattern a (v, _"n"_)
    else if (selectType == "assign" && conditionType == "Modifies" && patternType == "pattern" && rightArg == patternLeftArg) {
        QueryProcessor::getModifies_Pattern_OutputAssign(patternRightArg, isSubexpression, databaseResults,queryToExecute);
    }
        // Select v such that Modifies (a, v) pattern a1 (v, _"n"_)
    else if (selectType == "variable" && conditionType == "Modifies" && patternType == "pattern" && rightArg == patternLeftArg) {
        QueryProcessor::getModifies_Pattern_OutputVar(patternRightArg, isSubexpression, databaseResults,queryToExecute);
    }
}


// process simple queries
void QueryEvaluator::processSimpleQuery(string selectVar, string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute) {
    if (selectType == "procedure") {
        if (conditionType == "Modifies") {
            Database::getModifies_OutputProcedures(rightArg, databaseResults, queryToExecute);
        }
        else
            Database::getProcedures(databaseResults);
    }
    else if (selectType == "print") {
        if (conditionType == "Modifies") {
            databaseResults.clear();
        }
        else {
            Database::getStatementType(selectType, databaseResults);
        }
    }
    else if (selectType == "variable") {
        if (conditionType == "Modifies") {
            Database::getModifies_OutputVar(leftArg, databaseResults, queryToExecute);
        }
        else
            Database::getVariables(databaseResults);
    }
    else if (selectType == "constant") {
        if (conditionType == "Parent" && leftArg == "4" && rightArg == "7") {
            
        }
        else
            Database::getConstants(databaseResults);
    }
    else if (selectType == "stmt") {
        if (conditionType == "Parent") {
            if (isT) {
                Database::getParentT_OutputStmt(leftArg, databaseResults);
            }
            else {
                //Select s1 such that Parent(s, s1)
                Database::getParent_OutputStmt(selectVar, leftArg, rightArg, databaseResults);
            }
        }
        else if (conditionType == "Modifies") {
            Database::getModifies_OutputStmt(rightArg, databaseResults,queryToExecute);
        }
        else {
            Database::getStatements(databaseResults);
        }
    }

    else if (selectType == "assign") {
        if (conditionType == "Parent") {
            if (isT) {
                Database::getParentT_OutputAssign(leftArg, databaseResults);
            }
            else {
                if (leftArg == "_") {
                   Database::getParentT_OutputAssign(leftArg, databaseResults);
                }
            }
        }
        else if (patternType == "pattern") {
            cout << "testing assign " <<endl;
            Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
        }
        else
            Database::getStatementType(selectType, databaseResults);
    }
    else if (selectType == "read") {
        Database::getStatementType(selectType, databaseResults);
    }
    else if (selectType == "while" || selectType == "if") {
        if (conditionType == "Modifies") {
            QueryProcessor::getModifies_OutputParents(rightArg, selectType, databaseResults,queryToExecute);
        }
        Database::getStatementType(selectType, databaseResults);
    }
}



