#include "Database.h"
#include <unordered_map>

sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;

// method to connect to the database and initialize tables in the database
void Database::initialize() {
    // open a database connection and store the pointer into dbConnection
    sqlite3_open("database.db", &dbConnection);

    // drop existing tables (if any)
    const char* dropTablesSQL = "DROP TABLE IF EXISTS Modifies;"
                                "DROP TABLE IF EXISTS Pattern;"
                                "DROP TABLE IF EXISTS ParentChildRelation;"
                                "DROP TABLE IF EXISTS Variable;"
                                "DROP TABLE IF EXISTS Statement;"
                                "DROP TABLE IF EXISTS Constant;"
                                "DROP TABLE IF EXISTS Procedure;";
    sqlite3_exec(dbConnection, dropTablesSQL, NULL, 0, &errorMessage);

    // create Procedure table
    const char* createProcedureTableSQL = "CREATE TABLE Procedure ("
                                          "procedureName VARCHAR(255) PRIMARY KEY);";
    sqlite3_exec(dbConnection, createProcedureTableSQL, NULL, 0, &errorMessage);

    // create Statement table
    const char* createStatementTableSQL = "CREATE TABLE Statement ("
                                          "codeLine INT PRIMARY KEY,"
                                          "procedureName VARCHAR(255),"
                                          "statementType VARCHAR(50),"
                                          "statementContent VARCHAR(255),"
                                          "FOREIGN KEY (procedureName) REFERENCES Procedure(procedureName));";
    sqlite3_exec(dbConnection, createStatementTableSQL, NULL, 0, &errorMessage);

    // create Variable table
    const char* createVariableTableSQL = "CREATE TABLE Variable ("
                                         "variableName VARCHAR(255),"
                                         "statementCodeLine INT,"
                                         "FOREIGN KEY (statementCodeLine) REFERENCES Statement(codeLine));";
    sqlite3_exec(dbConnection, createVariableTableSQL, NULL, 0, &errorMessage);

    // create Constant table
    const char* createConstantTableSQL = "CREATE TABLE Constant ("
                                         "statementCodeLine INT,"
                                         "constantValue INT, "
                                         "FOREIGN KEY (statementCodeLine) REFERENCES Statement(codeLine));";
    sqlite3_exec(dbConnection, createConstantTableSQL, NULL, 0, &errorMessage);

    // create ParentChildRelation table
    const char* createParentChildRelationTableSQL = "CREATE TABLE ParentChildRelation ("
                                                    "parentStatementCodeLine INT,"
                                                    "childStatementCodeLine INT,"
                                                    "PRIMARY KEY (parentStatementCodeLine, childStatementCodeLine),"
                                                    "FOREIGN KEY (parentStatementCodeLine) REFERENCES Statement(codeLine));";
    sqlite3_exec(dbConnection, createParentChildRelationTableSQL, NULL, 0, &errorMessage);



    // create Modifies table
    const char* createModifiesTableSQL = "CREATE TABLE Modifies ("
                                         "statementCodeLine INT,"
                                         "variableName VARCHAR(255),"
                                         "FOREIGN KEY (statementCodeLine) REFERENCES Statement(codeLine),"
                                         "FOREIGN KEY (variableName) REFERENCES Variable(variableName));";
    sqlite3_exec(dbConnection, createModifiesTableSQL, NULL, 0, &errorMessage);

    // create Pattern table
    const char* createPatternTableSQL = "CREATE TABLE Pattern ("
                                        "statementCodeLine INT,"
                                        "LHSExpression VARCHAR(255),"
                                        "RHSExpression VARCHAR(255),"
                                        "FOREIGN KEY (statementCodeLine) REFERENCES Statement(codeLine));";
    sqlite3_exec(dbConnection, createPatternTableSQL, NULL, 0, &errorMessage);

    // initialize the result vector
    dbResults = vector<vector<string>>();
}

void Database::postProcessDbResults(vector<string>& results, int columnIndex) {
    for (const vector<string>& dbRow : dbResults) {  
            results.push_back(dbRow.at(columnIndex));    
    }
}

void Database::clear() {
    // Delete all records from each table
    const char* clearTablesSQL = "DELETE FROM Modifies;"
                                 "DELETE FROM Pattern;"
                                 "DELETE FROM ParentChildRelation;"
                                 "DELETE FROM Variable;"
                                 "DELETE FROM Statement;"
                                 "DELETE FROM Constant;"
                                 "DELETE FROM Procedure;";
    sqlite3_exec(dbConnection, clearTablesSQL, NULL, 0, &errorMessage);
}


// method to close the database connection
void Database::close() {
    sqlite3_close(dbConnection);
}

// method to insert a procedure into the database
void Database::insertProcedure(string procedureName) {
    string insertProcedureSQL = "INSERT INTO Procedure ('procedureName') VALUES ('" + procedureName + "');";
    sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
}

// method to get all the procedures from the database
void Database::getProcedures(vector<string>& results){
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the procedure table
    // The callback method is only used when there are results to be returned.
    string getProceduresSQL = "SELECT * FROM Procedure;";
    sqlite3_exec(dbConnection, getProceduresSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcessDbResults(results,0);
}

// method to get insert statement into the database
void Database::insertStatement(string procedureName, string statementType, string statementContent, int codeLine) {
    string insertStatementSQL = "INSERT INTO Statement (codeLine, procedureName, statementType, statementContent) VALUES ("
                                + to_string(codeLine) + ", '"
                                + procedureName + "', '"
                                + statementType + "', '"
                                + statementContent + "');";
    sqlite3_exec(dbConnection, insertStatementSQL.c_str(), NULL, 0, &errorMessage);
}

// method to get all the statements line from the database
void Database::getStatements(vector<string>& results) {
    dbResults.clear();

    string getStatementsSQL = "SELECT DISTINCT codeLine FROM Statement;";
    sqlite3_exec(dbConnection, getStatementsSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results,0);
}

// method to get all the statements line from the database by statement type eg. print, push
void Database::getStatementType(const string &statementType, vector<string>& results) {
    dbResults.clear();
    string getStatementsSQL = "SELECT codeLine FROM Statement WHERE statementType ='"
                               + statementType + "';";
    sqlite3_exec(dbConnection, getStatementsSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results,0);
}

// method to insert a variable into the database
void Database::insertVariable(string variableName, int statementCodeLine) {
    string insertVariableSQL = "INSERT INTO Variable (variableName, statementCodeLine) VALUES ('"
                               + variableName + "', '"
                               + to_string(statementCodeLine) + "');";
    sqlite3_exec(dbConnection, insertVariableSQL.c_str(), NULL, 0, &errorMessage);
}

// method to get all the variables from the database
void Database::getVariables(vector<string>& results) {
    dbResults.clear();

    string getVariablesSQL = "SELECT DISTINCT variableName FROM Variable;";
    sqlite3_exec(dbConnection, getVariablesSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results,0);
}

// method to insert a constant into the database
void Database::insertConstant(int statementCodeLine, int constantValue) {
    string insertConstantSQL = "INSERT INTO Constant (statementCodeLine,constantValue) VALUES ("
                               + to_string(statementCodeLine) + ","
                               + to_string(constantValue) + ");";

    sqlite3_exec(dbConnection, insertConstantSQL.c_str(), NULL, 0, &errorMessage);
}

// method to get all the constants from the database
void Database::getConstants(vector<string>& results) {
    dbResults.clear();  

    string getConstantsSQL = "SELECT constantValue FROM Constant;";
    sqlite3_exec(dbConnection, getConstantsSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results,0);
}

void Database::insertParentChildRelation(int parentStatementCodeLine, int childStatementCodeLine) {
    string insertSQL = "INSERT INTO ParentChildRelation (parentStatementCodeLine, childStatementCodeLine) VALUES ("
                       + to_string(parentStatementCodeLine) + ", "
                       + to_string(childStatementCodeLine) + ");";
    sqlite3_exec(dbConnection, insertSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::getParentChildRelations(vector<string>& results) {
    dbResults.clear();
    string getSQL = "SELECT parentStatementCodeLine, childStatementCodeLine FROM ParentChildRelation;";
    sqlite3_exec(dbConnection, getSQL.c_str(), callback, 0, &errorMessage);
    postProcessDbResults(results,0);
}




void Database::insertModifies(int statementCodeLine, const string& variableName) {
    string insertSQL = "INSERT INTO Modifies (statementCodeLine, variableName) VALUES ("
                       + to_string(statementCodeLine) + ", '"
                       + variableName + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), NULL, 0, &errorMessage);
}

//void Database::getModifies(vector<string>& results) {
//    dbResults.clear();
//    string getSQL = "SELECT statementCodeLine, variableName FROM Modifies;";
//    sqlite3_exec(dbConnection, getSQL.c_str(), callback, 0, &errorMessage);
//    postProcessDbResults(results,0);
//}

void Database::insertPattern(int statementCodeLine, const string& LHSExpression, const string& RHSExpression) {
    string insertSQL = "INSERT INTO Pattern (statementCodeLine, LHSExpression, RHSExpression) VALUES ("
                       + to_string(statementCodeLine) + ", '"
                       + LHSExpression + "', '"
                       + RHSExpression + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), NULL, 0, &errorMessage);
}

//void Database::getPatterns(vector<string>& results) {
//    dbResults.clear();
//    string getSQL = "SELECT statementCodeLine, LHSExpression, RHSExpression FROM Pattern;";
//    sqlite3_exec(dbConnection, getSQL.c_str(), callback, 0, &errorMessage);
//    postProcessDbResults(results,2);
//}





// callback method to put one row of results from the database into the dbResults vector
// This method is called each time a row of results is returned from the database
int Database::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    NotUsed = 0;
    vector<string> dbRow;

    // argc is the number of columns for this row of results
    // argv contains the values for the columns
    // Each value is pushed into a vector.
    for (int i = 0; i < argc; i++) {
        dbRow.push_back(argv[i]);
    }

    // The row is pushed to the vector for storing all rows of results
    dbResults.push_back(dbRow);

    return 0;
}



void Database::getModifies_OutputVar(string codeLine, vector<string>& results) {

    dbResults.clear();

    string getModifies_OutputVarSQL = "SELECT variableName FROM Modifies WHERE statementCodeLine ='"
        + codeLine + "';";

    sqlite3_exec(dbConnection, getModifies_OutputVarSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}

void Database::getModifies_OutputStmt(string rightArg, vector<string>& results) {

    dbResults.clear();

    string getModifies_OutputStmtSQL = "SELECT statementCodeLine FROM Modifies WHERE variableName = '"
        + rightArg + "';";

    sqlite3_exec(dbConnection, getModifies_OutputStmtSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}

void Database::getModifies_OutputProcedures(string rightArg, vector<string>& results) {

    dbResults.clear();

    string getModifies_OutputProceduresSQL = "SELECT DISTINCT s.procedureName FROM Statement s JOIN Modifies m ON m.statementCodeLine = s.codeLine WHERE m.variableName = '"
        + rightArg +"';";

    sqlite3_exec(dbConnection, getModifies_OutputProceduresSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}


void Database::getParentT_OutputStmt(string leftArg, vector<string>& results) {

    dbResults.clear();

    string getParentT_OutputStmtSQL;

    if (leftArg == "w") {
        getParentT_OutputStmtSQL = "SELECT p.childStatementCodeLine FROM ParentChildRelation p JOIN Statement s ON p.parentStatementCodeLine = s.codeLine WHERE s.statementType = 'while';";
        sqlite3_exec(dbConnection, getParentT_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    else if (leftArg == "i") {
        getParentT_OutputStmtSQL = "SELECT p.childStatementCodeLine FROM ParentChildRelation p JOIN Statement s ON p.parentStatementCodeLine = s.codeLine WHERE s.statementType = 'if';";
        sqlite3_exec(dbConnection, getParentT_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    postProcessDbResults(results, 0);
}

void Database::getParent_OutputStmt(string RightArg, vector<string>& results) {

    dbResults.clear();

    string getParent_OutputStmtSQL = "SELECT parentStatementCodeLine FROM ParentChildRelation WHERE childStatementCodeLine ='"
        + RightArg + "';";

    sqlite3_exec(dbConnection, getParent_OutputStmtSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}


void Database::getPattern_OutputStmt(string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& results) {
    dbResults.clear();

    string getPattern_OutputStmtSQL;

    if (patternLeftArg == "_" && patternRightArg == "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern;";
        sqlite3_exec(dbConnection, getPattern_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    // to fix leftArg space
    else if (isSubexpression && patternLeftArg != "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE RHSExpression like '%"
            + patternRightArg + "%' AND LHSExpression = '"
            + patternLeftArg + " ';";
        sqlite3_exec(dbConnection, getPattern_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    else if (isSubexpression && patternLeftArg == "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE RHSExpression like '%"
            + patternRightArg + "%';";
        sqlite3_exec(dbConnection, getPattern_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    else if (patternLeftArg == "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE RHSExpression = '"
            + patternRightArg + "';";
        sqlite3_exec(dbConnection, getPattern_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    //to fix leftArg space
    else if (patternRightArg == "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE LHSExpression = '"
            + patternLeftArg + " ';";
        sqlite3_exec(dbConnection, getPattern_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    //to fix leftArg space
    else if (patternLeftArg != "" && patternRightArg != "") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE LHSExpression = '"
            + patternLeftArg + " ' AND RHSExpression = '"
            + patternRightArg + "';";
        sqlite3_exec(dbConnection, getPattern_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
        postProcessDbResults(results, 0);
}

void Database::getCombo_ParentT_Pattern_OutputStmt(string res, vector<string>& results) {
    dbResults.clear();

    string getCombo_ParentT_Pattern_OutputStmtSQL = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation WHERE childStatementCodeLine in ("
        + res + ");";
    sqlite3_exec(dbConnection, getCombo_ParentT_Pattern_OutputStmtSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}

void Database::getCombo_Modifies_Pattern_OutputProcedure(string res, vector<string>& results) {
    dbResults.clear();

    string getCombo_Modifies_Pattern_OutputProcedureSQL = "SELECT DISTINCT procedureName FROM Statement WHERE codeLine in ("
        + res + ");";
    sqlite3_exec(dbConnection, getCombo_Modifies_Pattern_OutputProcedureSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}

