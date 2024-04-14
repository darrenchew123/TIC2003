#include "HandleSimpleQueries.h"

void HandleSimpleQueries::processSimpleQuery(string selectVar, string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute) {
    if (selectType == "procedure") {
        handleProcedureSelectType(conditionType, isT, leftArg, rightArg, databaseResults, queryToExecute);
    } else if (selectType == "print") {
        handlePrintSelectType(conditionType, isT, selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
    }else if (selectType == "variable") {
        handleVariableSelectType(conditionType, isT, selectVar, selectType, leftArg, rightArg, patternType, patternLeftArg ,patternRightArg, isSubexpression, databaseResults, queryToExecute);
    } else if (selectType == "constant") {
        handleConstantSelectType(conditionType,isT, selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
    }else if (selectType == "stmt") {
        handleStmtSelectType(conditionType, isT, selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
    } else if (selectType == "assign") {
        handleAssignSelectType(conditionType, isT, selectVar, selectType, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
    }    else if (selectType == "read") {
        handleReadSelectType(conditionType, isT, selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
    } else if (selectType == "while" || selectType == "if") {
        handleControlFlowSelectType(conditionType, isT, selectVar, selectType, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
    }
}

void HandleSimpleQueries::handleProcedureSelectType(const string& conditionType, bool isT, const string& leftArg, const string& rightArg, vector<string>& databaseResults, Query& queryToExecute) {
    if (conditionType == "Modifies") {
        Database::getModifies_OutputProcedures(rightArg, databaseResults, queryToExecute);
    } else if (conditionType == "Uses") {
        Database::getUses_OutputProcedures(leftArg, databaseResults, queryToExecute);
    } else if (conditionType == "Calls") {
        if (isT) {
            Database::getCallsT_OutputProcedures(leftArg, rightArg, databaseResults, queryToExecute);
        } else {
            Database::getCalls_OutputProcedures(leftArg, rightArg, databaseResults, queryToExecute);
        }
    } else {
        Database::getProcedures(databaseResults);
    }
}

void HandleSimpleQueries::handlePrintSelectType(const string& conditionType, bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, vector<string>& databaseResults, Query& queryToExecute) {
    if (conditionType == "Uses") {
        Database::getUses_OutputType(leftArg, rightArg, databaseResults, queryToExecute);
    } else if (conditionType == "Parent") {
        handleParentCondition(isT, selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
    } else {
        Database::getStatementType(selectType, databaseResults);
    }
}

void HandleSimpleQueries::handleParentCondition(bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, vector<string>& databaseResults, Query& queryToExecute) {
    if (isT) {
        cout << "ParentT" << endl;
        Database::getParentT(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
    } else {
        cout << "Parent" << endl;
        Database::getParent(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
    }
}

void HandleSimpleQueries::handleVariableSelectType(const string& conditionType, bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, const string& patternType, const string& patternLeftArg,const string& patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query& queryToExecute) {
    if (conditionType == "Modifies") {
        Database::getModifies_OutputVar(leftArg, databaseResults, queryToExecute);
    } else if (conditionType == "Parent") {
        handleParentCondition(isT, selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
    } else if (patternType == "pattern") {
        Database::getVariablesPattern(databaseResults, patternLeftArg ,patternRightArg, isSubexpression, queryToExecute);
    } else if (conditionType == "Uses") {
        Database::getUses_OutputVar(leftArg, databaseResults, queryToExecute);
    } else {
        Database::getVariables(databaseResults);
    }
}

void HandleSimpleQueries::handleConstantSelectType( string conditionType,bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, vector<string>& databaseResults, Query& queryToExecute) {
    if (conditionType == "Parent") {
        handleParentCondition(isT, selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
    } else {
        Database::getConstants(databaseResults);
    }
}

void HandleSimpleQueries::handleStmtSelectType(const string& conditionType, bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, vector<string>& databaseResults, Query& queryToExecute) {
    if (conditionType == "Parent") {
        handleParentCondition(isT, selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
    } else if (conditionType == "Uses") {
        Database::getUses_OutputStmt(leftArg, rightArg, databaseResults, queryToExecute);
    } else if (conditionType == "Modifies") {
        Database::getModifies_OutputStmt(rightArg, databaseResults, queryToExecute);
    } else {
        Database::getStatements(databaseResults);
    }
}

void HandleSimpleQueries::handleAssignSelectType(const string& conditionType, bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, const string& patternType, const string& patternLeftArg, const string& patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query& queryToExecute) {
    if (conditionType == "Parent") {
        handleParentCondition(isT, selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
    } else if (conditionType == "Uses") {
        Database::getUses_OutputType(leftArg, rightArg, databaseResults, queryToExecute);
    } else if (patternType == "pattern") {
        Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, databaseResults,
                                        queryToExecute);
    }else {
        Database::getStatementType(selectType, databaseResults);
    }
}

void HandleSimpleQueries::handleReadSelectType(const string& conditionType, bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, vector<string>& databaseResults, Query& queryToExecute) {
    if (conditionType == "Parent") {
        handleParentCondition(isT, selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
    } else {
        Database::getStatementType(selectType, databaseResults);
    }
}

void HandleSimpleQueries::handleControlFlowSelectType(const string& conditionType, bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, const string& patternType, const string& patternLeftArg, const string& patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query& queryToExecute) {
    if (conditionType == "Modifies") {
        Database::getModifies_OutputParents(selectType, databaseResults);
    } else if (conditionType == "Parent") {
        if (isT) {
            cout << "ParentT" << endl;
            Database::getParentT(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
        } else {
            cout << "Parent" << endl;
            Database::getParent(selectVar, selectType, leftArg, rightArg, databaseResults, queryToExecute);
        }
    } else if (patternType == "pattern") {
        Database::getPattern_OutputStmt(patternLeftArg, patternRightArg, isSubexpression, databaseResults, queryToExecute);
    }else {
        Database::getStatementType(selectType, databaseResults);
    }
}



