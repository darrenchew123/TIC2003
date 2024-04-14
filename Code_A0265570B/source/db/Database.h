#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "sqlite3.h"
#include "../pql/Query.h"
#include "ParentT_HelperFunctions.h"
#include "Parent_HelperFunctions.h"



using namespace std;

// The Database has to be a static class due to various constraints.
// It is advisable to just add the insert / get functions based on the given examples.
class Database {
public:
    static void initialize();

    static void clear();

    static void close();

    static void executeAndProcessSQL(const string& sqlQuery, vector<string>& results);

    static void postProcessDbResults(vector<string>& results, int columnIndex);

    static vector<string> findCommonStrings(vector<string>& arr1, vector<string>& arr2);

    static void insertProcedure(string procedureName);

    static void getProcedures(vector<string>& results);

    static void insertStatement(string procedureName, string statementType, string statementContent, int codeLine);

    static void getStatements(vector<string>& results);

    static void getStatementType(const string& statementType, vector<string>& results);

    static void insertVariable(string variableName, int statementCodeLine);

    static void getVariables(vector<string>& results);

    static void getVariablesPattern(vector<string>& results, string lhsArgs ,string rhsArgs, bool isSubExpression, Query query);

    static void insertConstant(int statementCodeLine, int constantValue);

    static void getConstants(vector<string>& results);

    static void insertParentChildRelation(int parentStatementCodeLine, int childStatementCodeLine);

    static void getParentChildRelations(vector<string>& results);

    static void insertAncestorRelation(int ancestorStatementCodeLine, int childStatementCodeLine);

    static void getAncestorRelation(vector<string>& results);

    static void insertModifies(int statementCodeLine, const string& variableName);

    static void getModifies(vector<string>& results);

    static void insertPattern(int statementCodeLine, const string& LHSExpression, const string& RHSExpression);

    static void getPatterns(vector<string>& results);

    static void insertUses(int statementCodeLine, const string& variableName);

    static void getUses(vector<string>& results);

    static void insertCalls(const string& caller, const string& callee);

    static void insertCallsT(const string& caller, const string& callee);

    static void getModifies_OutputVar(string leftArg, vector<string>& results, Query queryToExecute);

    static void getModifies_OutputStmt(string rightArg, vector<string>& results, Query queryToExecute);

    static void getModifies_OutputParents(string leftArg, vector<string>& results);

    static void getModifies_OutputProcedures(string rightArg, vector<string>& results, Query queryToExecute);

    static void getStatements_OutputAssign(vector<string>& results);

    static void getPattern_OutputStmt(string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& results, Query queryToExecute);

    static void getCombo_Modifies_Pattern_OutputProcedure(string res, vector<string>& results);

    static void getCombo_Modifies_Pattern_OutputAssign(string res, vector<string>& results);

    static void getCombo_Modifies_Pattern_OutputVar(string res, vector<string>& results);

    static void getUses_OutputVar(string leftArg, vector<string>& results, Query queryToExecute);

    static void getUses_OutputStmt(string leftArg, string rightArg, vector<string>& results, Query queryToExecute);

    static void getUses_OutputProcedures(string leftArg, vector<string>& results, Query queryToExecute);

    static void getUses_OutputType(string leftArg, string rightArg,vector<string>& results, Query queryToExecute);

    static void getCalls_OutputProcedures(string leftArg, string rightArg, vector<string>& results, Query queryToExecute);

    static void getCallsT_OutputProcedures(string leftArg, string rightArg, vector<string>& results, Query queryToExecute);

    static void getParent(string selectVar, string selectType, string leftArg, string rightArg, vector<string>& results, Query query);

    static void getParentT(string selectVar, string selectType, string leftArg, string rightArg, vector<string>& results, Query query);

    static bool executeCheckQuery(string sqlQuery, vector<string> params);

    static bool checkCallsRelationship(string caller, string callee);

    static bool checkParentRelationship(string parent, string child);

    static bool checkCallsTRelationship(string caller, string callee);

    static bool checkParentTRelationship(string parent, string child);

    static bool checkModifiesRelationship(string statementCodeLine, string variableName, string statementType);

    static void prepareContext_Parent(const std::unordered_map<std::string, std::string>& declaredVariables,
                                      const std::string& leftArg, const std::string& rightArg,
                                      bool& isLhsSynonym, std::string& lhsSynType,
                                      bool& isRhsSynonym, std::string& rhsSynType,
                                      bool& ancestorExists);

private:
    static sqlite3* dbConnection;

    static vector<vector<string>> dbResults;

    static char* errorMessage;

    static int callback(void* NotUsed, int argc, char** argv, char** azColName);


};

