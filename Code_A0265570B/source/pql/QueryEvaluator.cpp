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
    string conditionType, leftArg, rightArg, selectType, selectVar, patternType, patternLeftArg, patternRightArg;
    bool isT = false, isSubexpression = false, isMultipleCond = false, isMultiSelect = false;

    tk.tokenize(query, tokens);
    Query queryToExecute = QueryParser::parser(tokens);

    selectType = queryToExecute.selectType;
    selectVar = queryToExecute.selectVar;
    debugPrintQuery(queryToExecute);

    extractConditions(queryToExecute, conditionType, isT, leftArg, rightArg);
    extractPatterns(queryToExecute, patternType, patternLeftArg, patternRightArg, isSubexpression);
    determineQueryComplexity(queryToExecute, isMultipleCond, isMultiSelect);

    if (isMultiSelect) {
        cout << "multi select" << endl;
        HandleMultipleSelect::processMultiSelectQuery( conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute, isMultipleCond);
    }
    else if (isMultipleCond) {
        cout << "single select + multicond" << endl;
        HandleMultipleConditions::processSingleSelectMultiCond(selectVar, selectType, conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
    } else {
        cout << "single select + single cond" << endl;
        HandleSimpleQueries::processSimpleQuery(selectVar, selectType, conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
    }
    output.insert(output.end(), databaseResults.begin(), databaseResults.end());
}



void QueryEvaluator::extractConditions(const Query& queryToExecute, string& conditionType, bool& isT, string& leftArg, string& rightArg) {
    if (!queryToExecute.conditions.empty()) {
        conditionType = queryToExecute.conditions[0].conditionType;
        isT = queryToExecute.conditions[0].isT;
        leftArg = queryToExecute.conditions[0].leftArg;
        rightArg = queryToExecute.conditions[0].rightArg;
        cout << "left arg " << leftArg << " right arg " << rightArg << " select-type " << queryToExecute.selectType << endl;
    }
}

void QueryEvaluator::extractPatterns(const Query& queryToExecute, string& patternType, string& patternLeftArg, string& patternRightArg, bool& isSubexpression) {
    if (!queryToExecute.patterns.empty()) {
        patternType = queryToExecute.patterns[0].patternType;
        patternLeftArg = queryToExecute.patterns[0].patternLeftArg;
        patternRightArg = queryToExecute.patterns[0].patternRightArg;
        isSubexpression = queryToExecute.patterns[0].isSubexpression;
    }
}

void QueryEvaluator::determineQueryComplexity(const Query& queryToExecute, bool& isMultipleCond, bool& isMultiSelect) {
    int condSize = queryToExecute.conditions.size();
    int patternSize = queryToExecute.patterns.size();
    int totalSize = condSize + patternSize;

    isMultipleCond = totalSize > 1;
    isMultiSelect = !queryToExecute.multiSelectVar.empty();

    cout << "isMultipleCond= " << isMultipleCond << " isMultiSelect= " << isMultiSelect << endl;
}

void QueryEvaluator::debugPrintQuery(const Query& queryToExecute) {
    cout << "Conditions:\n";
    for (const auto& condition : queryToExecute.conditions) {
        cout << "  Condition Type: " << condition.conditionType
             << ", Left Arg: " << condition.leftArg
             << ", Right Arg: " << condition.rightArg
             << ", isT: " << condition.isT << "\n";
    }

    cout << "Patterns:\n";
    for (const auto& pattern : queryToExecute.patterns) {
        cout << "  Pattern Type: " << pattern.patternType
             << ", Pattern Var: " << pattern.patternVar
             << ", Left Arg: " << pattern.patternLeftArg
             << ", Right Arg: " << pattern.patternRightArg
             << ", isSubexpression: " << pattern.isSubexpression << "\n";
    }

    cout << "Declared Variables:\n";
    for (const auto& var : queryToExecute.declaredVariables) {
        cout << "  Var Name: " << var.first << ", Var Type: " << var.second << "\n";
    }

    cout << "Multi Select Types:\n";
    for (const auto& type : queryToExecute.multiSelectType) {
        cout << "  Type: " << type << "\n";
    }

    cout << "Multi Select Vars:\n";
    for (const auto& var : queryToExecute.multiSelectVar) {
        cout << "  Var: " << var << "\n";
    }

    cout << "Select Type: " << queryToExecute.selectType << "\n";
    cout << "Select Var: " << queryToExecute.selectVar << "\n";
}
