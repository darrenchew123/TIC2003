#include "Database.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>

sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;

// method to connect to the database and initialize tables in the database
void Database::initialize() {
    // open a database connection and store the pointer into dbConnection
    sqlite3_open("database.db", &dbConnection);

    // drop existing tables (if any)
    const char* dropTablesSQL = "DROP TABLE IF EXISTS Modifies;"
        "DROP TABLE IF EXISTS Uses;"
        "DROP TABLE IF EXISTS Pattern;"
        "DROP TABLE IF EXISTS ParentChildRelation;"
        "DROP TABLE IF EXISTS AncestorRelation;"
        "DROP TABLE IF EXISTS Variable;"
        "DROP TABLE IF EXISTS Statement;"
        "DROP TABLE IF EXISTS Constant;"
        "DROP TABLE IF EXISTS Call;"
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

    // create AncestorRelation table
    const char* createAncestorRelationSQL = "CREATE TABLE AncestorRelation ("
                                                    "ancestorStatementCodeLine INT,"
                                                    "childStatementCodeLine INT,"
                                                    "PRIMARY KEY (ancestorStatementCodeLine, childStatementCodeLine),"
                                                    "FOREIGN KEY (ancestorStatementCodeLine) REFERENCES Statement(codeLine));";
    sqlite3_exec(dbConnection, createAncestorRelationSQL, NULL, 0, &errorMessage);

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

    // create Uses table
    const char* createUsesTableSQL = "CREATE TABLE Uses ("
                                     "statementCodeLine INT,"
                                     "variableName VARCHAR(255),"
                                     "FOREIGN KEY (statementCodeLine) REFERENCES Statement(codeLine),"
                                     "FOREIGN KEY (variableName) REFERENCES Variable(variableName));";
    sqlite3_exec(dbConnection, createUsesTableSQL, NULL, 0, &errorMessage);

    // Create Call table
    const char* createCallTableSQL =
            "CREATE TABLE Call ("
            "procedureCaller VARCHAR(255),"
            "procedureCallee VARCHAR(255),"
            "FOREIGN KEY (procedureCaller) REFERENCES Procedure(procedureName),"
            "FOREIGN KEY (procedureCallee) REFERENCES Procedure(procedureName));";
    sqlite3_exec(dbConnection, createCallTableSQL, NULL, 0, &errorMessage);

    // initialize the result vector
    dbResults = vector<vector<string>>();
}

void Database::postProcessDbResults(vector<string>& results, int columnIndex) {
    for (const vector<string>& dbRow : dbResults) {
        results.push_back(dbRow.at(columnIndex));
    }
}

vector<string> Database::findCommonStrings(vector<string>& arr1, vector<string>& arr2) {
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

void Database::clear() {
    // Delete all records from each table
    const char* clearTablesSQL = "DELETE FROM Modifies;"
        "DELETE FROM Uses;"
        "DELETE FROM Call;"
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
void Database::getProcedures(vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the procedure table
    // The callback method is only used when there are results to be returned.
    string getProceduresSQL = "SELECT * FROM Procedure;";
    sqlite3_exec(dbConnection, getProceduresSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcessDbResults(results, 0);
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

    postProcessDbResults(results, 0);
}

// method to get all the statements line from the database by statement type eg. print, push
void Database::getStatementType(const string& selectType, vector<string>& results) {
    dbResults.clear();

    string getStatementsSQL = "SELECT codeLine FROM Statement WHERE statementType ='"
        + selectType + "';";
    sqlite3_exec(dbConnection, getStatementsSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
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

    postProcessDbResults(results, 0);
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

    string getConstantsSQL = "SELECT DISTINCT constantValue FROM Constant;";
    sqlite3_exec(dbConnection, getConstantsSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
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
    postProcessDbResults(results, 0);
}

void Database::insertAncestorRelation(int ancestorStatementCodeLine, int childStatementCodeLine) {
    string insertSQL = "INSERT INTO AncestorRelation (ancestorStatementCodeLine, childStatementCodeLine) VALUES ("
                       + to_string(ancestorStatementCodeLine) + ", "
                       + to_string(childStatementCodeLine) + ");";
    sqlite3_exec(dbConnection, insertSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::getAncestorRelation(vector<string>& results) {
    dbResults.clear();
    string getSQL = "SELECT ancestorStatementCodeLine, childStatementCodeLine FROM AncestorRelation;";
    sqlite3_exec(dbConnection, getSQL.c_str(), callback, 0, &errorMessage);
    postProcessDbResults(results, 0);
}


void Database::insertModifies(int statementCodeLine, const string& variableName) {
    string insertSQL = "INSERT INTO Modifies (statementCodeLine, variableName) VALUES ("
        + to_string(statementCodeLine) + ", '"
        + variableName + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::getModifies(vector<string>& results) {
    dbResults.clear();
    string getSQL = "SELECT statementCodeLine, variableName FROM Modifies;";
    sqlite3_exec(dbConnection, getSQL.c_str(), callback, 0, &errorMessage);
    postProcessDbResults(results,0);
}

void Database::insertPattern(int statementCodeLine, const string& LHSExpression, const string& RHSExpression) {
    string insertSQL = "INSERT INTO Pattern (statementCodeLine, LHSExpression, RHSExpression) VALUES ("
        + to_string(statementCodeLine) + ", '"
        + LHSExpression + "', '"
        + RHSExpression + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::getPatterns(vector<string>& results) {
    dbResults.clear();
    string getSQL = "SELECT statementCodeLine, LHSExpression, RHSExpression FROM Pattern;";
    sqlite3_exec(dbConnection, getSQL.c_str(), callback, 0, &errorMessage);
    postProcessDbResults(results,2);
}

void Database::insertUses(int statementCodeLine, const string& variableName) {
    string insertSQL = "INSERT INTO Uses (statementCodeLine, variableName) VALUES ("
                       + to_string(statementCodeLine) + ", '"
                       + variableName + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::getUses(vector<string>& results) {
    dbResults.clear();
    string getSQL = "SELECT statementCodeLine, variableName FROM Uses;";
    sqlite3_exec(dbConnection, getSQL.c_str(), callback, 0, &errorMessage);
    postProcessDbResults(results,0);
}

void Database::insertCalls(const string& caller, const string& callee) {
    string insertCallSQL = "INSERT INTO Call (procedureCaller, procedureCallee) VALUES ('"
                           + caller + "', '"
                           + callee + "');";
    sqlite3_exec(dbConnection, insertCallSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::getUses_OutputVar(string leftArg, vector<string>& results) {

    dbResults.clear();

    string getUses_OutputVar = "SELECT variableName FROM Uses WHERE statementCodeLine ='"
                               + leftArg + "';";

    sqlite3_exec(dbConnection, getUses_OutputVar.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow : dbResults) {
        string var = dbRow.at(0);
        results.push_back(var);
    }
}



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





void Database::getModifies_OutputVar(string leftArg, vector<string>& results, Query queryToExecute) {

    dbResults.clear();
    string getModifies_OutputVarSQL;
    if(queryToExecute.declaredVariables[leftArg]=="stmt" || queryToExecute.declaredVariables[leftArg]=="procedure"){
        getModifies_OutputVarSQL ="SELECT distinct variableName FROM Modifies;";
    }
    else if(queryToExecute.declaredVariables[leftArg]=="read"){
        getModifies_OutputVarSQL ="SELECT m.variableName \n"
                                  "FROM Modifies AS m \n"
                                  "WHERE m.statementCodeLine IN (\n"
                                  "    SELECT s.codeLine \n"
                                  "    FROM Statement AS s \n"
                                  "    WHERE s.statementType = \"read\"\n"
                                  ");";
    }
    else{
        getModifies_OutputVarSQL = "SELECT variableName FROM Modifies WHERE statementCodeLine ='"
                                   + leftArg + "';";
    }


    sqlite3_exec(dbConnection, getModifies_OutputVarSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}

void Database::getModifies_OutputStmt(string rightArg, vector<string>& results, Query queryToExecute) {

    dbResults.clear();
    string getModifies_OutputStmtSQL;
    if(queryToExecute.declaredVariables[rightArg]=="variable" || rightArg == "_"){
        getModifies_OutputStmtSQL = "SELECT statementCodeLine FROM Modifies;";
    }
    else {
        getModifies_OutputStmtSQL = "SELECT statementCodeLine FROM Modifies WHERE variableName = '"
                                           + rightArg + "';";
    }

    sqlite3_exec(dbConnection, getModifies_OutputStmtSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}

void Database::getModifies_OutputProcedures(string rightArg, vector<string>& results, Query queryToExecute) {

    dbResults.clear();
    string getModifies_OutputProceduresSQL;
    if(queryToExecute.declaredVariables[rightArg]=="variable"){
        getModifies_OutputProceduresSQL = "SELECT procedureName FROM procedure";
    }
    else{
        getModifies_OutputProceduresSQL = "SELECT DISTINCT s.procedureName FROM Statement s JOIN Modifies m ON m.statementCodeLine = s.codeLine WHERE m.variableName = '"
                                                 + rightArg + "';";
    }



    sqlite3_exec(dbConnection, getModifies_OutputProceduresSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}

//void Database::getUses_OutputVar(string leftArg, vector<string>& results) {
//
//    dbResults.clear();
//
//    string getUses_OutputVar = "SELECT variableName FROM Uses WHERE statementCodeLine ='"
//        + leftArg + "';";
//
//    sqlite3_exec(dbConnection, getUses_OutputVar.c_str(), callback, 0, &errorMessage);
//
//    for (vector<string> dbRow : dbResults) {
//        string var = dbRow.at(0);
//        results.push_back(var);
//    }
//}

void Database::getParentT_OutputStmt(string leftArg, vector<string>& results) {

    unordered_set<string> parents;

    dbResults.clear();
    string getParentCodeLine = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation;";
    sqlite3_exec(dbConnection,getParentCodeLine.c_str(), callback, 0, &errorMessage);
    for (vector<string> dbRow : dbResults) {
        parents.insert(dbRow.at(0));
    }

    unordered_set<string> visited;

    queue<string> q;


    //to consider validation
    if (leftArg == "w") {
        dbResults.clear();
        string getChildrenCodeline = "SELECT p.childStatementCodeLine FROM ParentChildRelation p JOIN Statement s ON p.parentStatementCodeLine = s.codeLine WHERE s.statementType = 'while';";
        sqlite3_exec(dbConnection, getChildrenCodeline.c_str(), callback, 0, &errorMessage);
        for (vector<string> dbRow : dbResults) {
            q.push(dbRow.at(0));
        }
    }
    else if (leftArg == "i") {
        dbResults.clear();
        string getChildrenCodeline = "SELECT p.childStatementCodeLine FROM ParentChildRelation p JOIN Statement s ON p.parentStatementCodeLine = s.codeLine WHERE s.statementType = 'if';";
        sqlite3_exec(dbConnection, getChildrenCodeline.c_str(), callback, 0, &errorMessage);
        for (vector<string> dbRow : dbResults) {
            q.push(dbRow.at(0));
        }
    }

        //considering all parent type
    else if (leftArg == "_") {
        dbResults.clear();
        string getChildrenCodeline = "SELECT childStatementCodeLine FROM ParentChildRelation;";
        sqlite3_exec(dbConnection, getChildrenCodeline.c_str(), callback, 0, &errorMessage);
        for (vector<string> dbRow : dbResults) {
            q.push(dbRow.at(0));
        }
    }

    while (!q.empty()) {

        string curr = q.front();
        q.pop();

        if (parents.count(curr)) {

            dbResults.clear();
            string getChildrenCodeline = "SELECT childStatementCodeLine FROM ParentChildRelation WHERE parentStatementCodeLine = '"
                                            + curr + "';";
            sqlite3_exec(dbConnection, getChildrenCodeline.c_str(), callback, 0, &errorMessage);
            for (vector<string> dbRow : dbResults) {
                q.push(dbRow.at(0));
            }
            if (!visited.count(curr)) {
                results.push_back(curr);
                visited.insert(curr);
            }
        }
        else {
            if (!visited.count(curr)) {
                results.push_back(curr);
                visited.insert(curr);
            }
        }

    }
}

void Database::getStatements_OutputAssign(vector<string>& results) {

    dbResults.clear();

    string getStatements_OutputAssignSQL = "SELECT codeLine FROM Statement WHERE statementType = 'assign';";
    sqlite3_exec(dbConnection, getStatements_OutputAssignSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}

void Database::getParentT_OutputAssign(string leftArg, vector<string> &results) {
    vector<string> arr1;
    Database::getParentT_OutputStmt(leftArg, arr1);
    vector<string> arr2;
    Database::getStatements_OutputAssign(arr2);
    vector<string> commonStrings = Database::findCommonStrings(arr1, arr2);
    results = commonStrings;
}

void Database::getParent_OutputStmt(string selectVar, string leftArg, string rightArg, vector<string>& results) {

    dbResults.clear();

    string getParent_OutputStmtSQL;

    //test case: Select s1 such that Parent(s, s1)
    if (leftArg == selectVar && rightArg != selectVar) {
        getParent_OutputStmtSQL = "SELECT parentStatementCodeLine FROM ParentChildRelation WHERE childStatementCodeLine ='"
            + rightArg + "';";
        sqlite3_exec(dbConnection, getParent_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }

    //test case: Select s1 such that Parent(s, s1)
    //Select s such that Parent(s, _)
    if (leftArg == selectVar && rightArg == "_") {
        getParent_OutputStmtSQL = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation;";

        sqlite3_exec(dbConnection, getParent_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    if (leftArg == selectVar && rightArg != "_") {
        getParent_OutputStmtSQL = "SELECT parentStatementCodeLine FROM ParentChildRelation WHERE childStatementCodeLine ='"
            + rightArg + "';";

        sqlite3_exec(dbConnection, getParent_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    //test case: Select s such that Parent(s, s1)
    else if (leftArg == selectVar) { // LHS = selectVar -> return parents
        getParent_OutputStmtSQL = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation;";

        sqlite3_exec(dbConnection, getParent_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    else if (leftArg != selectVar && rightArg == selectVar) {
        getParent_OutputStmtSQL = "SELECT childStatementCodeLine FROM ParentChildRelation WHERE parentStatementCodeLine ='"
            + leftArg + "';";
        sqlite3_exec(dbConnection, getParent_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    //test case: Select s1 such that Parent(s, s1)
    else if (rightArg == selectVar) { // RHS = selectVar -> return children
        getParent_OutputStmtSQL = "SELECT childStatementCodeLine FROM ParentChildRelation;";

        sqlite3_exec(dbConnection, getParent_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    
    else {
        getParent_OutputStmtSQL = "SELECT parentStatementCodeLine FROM ParentChildRelation WHERE childStatementCodeLine ='"
            + rightArg + "';";

        sqlite3_exec(dbConnection, getParent_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    postProcessDbResults(results, 0);
}


void Database::getPattern_OutputStmt(string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& results, Query queryToExecute) {
    dbResults.clear();

    string getPattern_OutputStmtSQL;

    cout << "patternLeftArg " << patternLeftArg << " patternRightArg " << patternRightArg;

    if (patternLeftArg == "_" && patternRightArg == "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern;";
        sqlite3_exec(dbConnection, getPattern_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    else if (patternRightArg == "_" && queryToExecute.declaredVariables[patternLeftArg]=="variable") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE LHSExpression IN (SELECT variableName FROM Variable)";
        sqlite3_exec(dbConnection, getPattern_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    else if (isSubexpression && patternLeftArg != "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE RHSExpression like '%"
            + patternRightArg + "%' AND LHSExpression = '"
            + patternLeftArg + "';";
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
    else if (patternRightArg == "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE LHSExpression = '"
            + patternLeftArg + "';";
        sqlite3_exec(dbConnection, getPattern_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    else if (patternLeftArg != "" && patternRightArg != "") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE LHSExpression = '"
            + patternLeftArg + "' AND RHSExpression = '"
            + patternRightArg + "';";
        sqlite3_exec(dbConnection, getPattern_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    postProcessDbResults(results, 0);
}

void Database::getCombo_ParentT_Pattern_OutputStmt(string res, vector<string>& results) {
    dbResults.clear();

    string getDirectParents = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation WHERE childStatementCodeLine in ("
        + res + ");";
    sqlite3_exec(dbConnection, getDirectParents.c_str(), callback, 0, &errorMessage);

    queue <string> q;

    for (vector<string> parent : dbResults) {
        //push direct parents to queue
        q.push(parent.at(0));
    }

    //hash all children
    unordered_set<string> children;
    dbResults.clear();
    string getChildrenCodeLine = "SELECT DISTINCT childStatementCodeLine FROM ParentChildRelation;";
    sqlite3_exec(dbConnection, getChildrenCodeLine.c_str(), callback, 0, &errorMessage);
    for (vector<string> dbRow : dbResults) {
        children.insert(dbRow.at(0));
    }

    while (!q.empty()) {

        string curr = q.front();
        q.pop();

        results.push_back(curr);

        if (children.count(curr)) {

            dbResults.clear();
            string getParentCodeline = "SELECT parentStatementCodeLine FROM ParentChildRelation WHERE childStatementCodeLine = '"
                + curr + "';";
            sqlite3_exec(dbConnection, getParentCodeline.c_str(), callback, 0, &errorMessage);
            for (vector<string> dbRow : dbResults) {
                q.push(dbRow.at(0));
            }
        }
    }

}

void Database::getXTypeOfParents_OutputStmt(string selectType, vector<string>& results) {

    dbResults.clear();
    /*string getXTypeOfParents_OutputStmtSQL;
    if (selectType == "w") {
        getXTypeOfParents_OutputStmtSQL = "SELECT codeLine FROM Statement WHERE statementType = 'while';";
        sqlite3_exec(dbConnection, getXTypeOfParents_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }
    else if (selectType == "i") {
        getXTypeOfParents_OutputStmtSQL = "SELECT codeLine FROM Statement WHERE statementType = 'if';";
        sqlite3_exec(dbConnection, getXTypeOfParents_OutputStmtSQL.c_str(), callback, 0, &errorMessage);
    }*/

    string getXTypeOfParents_OutputStmtSQL = "SELECT codeLine FROM Statement WHERE statementType = '" 
                                              + selectType + "'; ";
    sqlite3_exec(dbConnection, getXTypeOfParents_OutputStmtSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}

void Database::getModifies_OutputParents(string selectType, string ParentLines, vector<string>& results) {

    dbResults.clear();

    string getModifies_OutputParentsSQL = "SELECT DISTINCT s.codeLine FROM Statement s JOIN ParentChildRelation p ON s.codeLine = p.parentStatementCodeLine WHERE s.statementType = '"
                                          + selectType + "' AND s.codeLine IN ("
                                          + ParentLines + ");";
    sqlite3_exec(dbConnection, getModifies_OutputParentsSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);

}

void Database::getCombo_Modifies_Pattern_OutputProcedure(string res, vector<string>& results) {
    dbResults.clear();

    string getCombo_Modifies_Pattern_OutputProcedureSQL = "SELECT DISTINCT procedureName FROM Statement WHERE codeLine in ("
        + res + ");";
    sqlite3_exec(dbConnection, getCombo_Modifies_Pattern_OutputProcedureSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}

void Database::getCombo_Modifies_Pattern_OutputAssign(string res, vector<string>& results) {
    dbResults.clear();

    string getCombo_Modifies_Pattern_OutputAssignSQL = "SELECT statementCodeLine FROM Modifies WHERE statementCodeLine in ("
                                                       + res + "); ";
    sqlite3_exec(dbConnection, getCombo_Modifies_Pattern_OutputAssignSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}

void Database::getCombo_Modifies_Pattern_OutputVar(string res, vector<string>& results) {
    dbResults.clear();

    string getCombo_Modifies_Pattern_OutputVarSQL = "SELECT variableName FROM Modifies WHERE statementCodeLine in ("
                                                    + res + "); ";
    sqlite3_exec(dbConnection, getCombo_Modifies_Pattern_OutputVarSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}


