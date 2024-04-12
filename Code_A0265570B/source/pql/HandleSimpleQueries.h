#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../db/Database.h"
#include "../utils/Tokenizer.h"
#include "QueryParser.h"
#include "QueryProcessor.h"
#include "Query.h"

class HandleSimpleQueries {
public:
    static void processSimpleQuery(string selectVar, string selectType, string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute);
    static void handleProcedureSelectType(const string& conditionType, bool isT, const string& leftArg, const string& rightArg, vector<string>& databaseResults, Query& queryToExecute);
    static void handlePrintSelectType(const string& conditionType, bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, vector<string>& databaseResults, Query& queryToExecute);
    static void handleParentCondition(bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, vector<string>& databaseResults, Query& queryToExecute);
    static void handleVariableSelectType(const string& conditionType, bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, const string& patternType, const string& patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query& queryToExecute);
    static void handleConstantSelectType( string conditionType,bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, vector<string>& databaseResults, Query& queryToExecute);
    static void handleStmtSelectType(const string& conditionType, bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, vector<string>& databaseResults, Query& queryToExecute);
    static void handleAssignSelectType(const string& conditionType, bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, const string& patternType, const string& patternLeftArg, const string& patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query& queryToExecute);
    static void handleReadSelectType(const string& conditionType, bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, vector<string>& databaseResults, Query& queryToExecute);
    static void handleControlFlowSelectType(const string& conditionType, bool isT, const string& selectVar, const string& selectType, const string& leftArg, const string& rightArg, const string& patternType, const string& patternLeftArg, const string& patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query& queryToExecute);

};
