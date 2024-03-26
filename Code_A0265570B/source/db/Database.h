#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"

using namespace std;

// The Database has to be a static class due to various constraints.
// It is advisable to just add the insert / get functions based on the given examples.
class Database {
public:
    static void initialize();

    static void clear();

    static void close();

    static void postProcessDbResults(vector<string>& results, int columnIndex);

    static vector<string> findCommonStrings(vector<string>& arr1, vector<string>& arr2);

    static void insertProcedure(string procedureName);

    static void getProcedures(vector<string>& results);

    static void insertStatement(string procedureName, string statementType, string statementContent, int codeLine);

    static void getStatements(vector<string>& results);

    static void getStatementType(const string& statementType, vector<string>& results);

    static void insertVariable(string variableName, int statementCodeLine);

    static void getVariables(vector<string>& results);

    static void insertConstant(int statementCodeLine, int constantValue);

    static void getConstants(vector<string>& results);

    static void insertParentChildRelation(int parentStatementCodeLine, int childStatementCodeLine);

    static void getParentChildRelations(vector<string>& results);

    static void insertModifies(int statementCodeLine, const string& variableName);

    static void getModifies(vector<string>& results);

    static void insertPattern(int statementCodeLine, const string& LHSExpression, const string& RHSExpression);

    static void getPatterns(vector<string>& results);

    static void getModifies_OutputVar(string codeLine, vector<string>& results);

    static void getModifies_OutputStmt(string rightArg, vector<string>& results);

    static void getModifies_OutputParents(string selectType, string ParentLines, vector<string>& results);

    static void getModifies_OutputProcedures(string rightArg, vector<string>& results);

    static void getParentT_OutputStmt(string leftArg, vector<string>& results);

    static void getParentT_OutputAssign(string leftArg, vector<string>& results);

    static void getStatements_OutputAssign(vector<string>& results);

    static void getParent_OutputStmt(string RightArg, vector<string>& results);

    static void getPattern_OutputStmt(string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& results);

    static void getCombo_ParentT_Pattern_OutputStmt(string res, vector<string>& results);

    static void getXTypeOfParents_OutputStmt(string selectType, vector<string> &results);

    static void getCombo_Modifies_Pattern_OutputProcedure(string res, vector<string>& results);

    static void getCombo_Modifies_Pattern_OutputAssign(string res, vector<string>& results);

    static void getCombo_Modifies_Pattern_OutputVar(string res, vector<string>& results);

private:
    static sqlite3* dbConnection;

    static vector<vector<string>> dbResults;

    static char* errorMessage;

    static int callback(void* NotUsed, int argc, char** argv, char** azColName);


};

