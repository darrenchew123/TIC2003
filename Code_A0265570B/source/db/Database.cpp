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
        "DROP TABLE IF EXISTS CallT;"
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

    // Create CallT table
    const char* createCallTTableSQL =
            "CREATE TABLE CallT ("
            "procedureCaller VARCHAR(255),"
            "procedureCallee VARCHAR(255),"
            "FOREIGN KEY (procedureCaller) REFERENCES Procedure(procedureName),"
            "FOREIGN KEY (procedureCallee) REFERENCES Procedure(procedureName));";
    sqlite3_exec(dbConnection, createCallTTableSQL, NULL, 0, &errorMessage);

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

void Database::executeAndProcessSQL(const string& sqlQuery, vector<string>& results) {
    dbResults.clear();
    sqlite3_exec(dbConnection, sqlQuery.c_str(), callback, 0, &errorMessage);

    if(errorMessage != nullptr) {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free((void*)errorMessage);
    } else {
        postProcessDbResults(results, 0);
    }
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
    string getStatementsSQL = "SELECT DISTINCT codeLine FROM Statement;";
    executeAndProcessSQL(getStatementsSQL,results);
}

// method to get all the statements line from the database by statement type eg. print, push
void Database::getStatementType(const string& selectType, vector<string>& results) {
    string getStatementsSQL = "SELECT codeLine FROM Statement WHERE statementType ='"
        + selectType + "';";
    executeAndProcessSQL(getStatementsSQL,results);
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
    string getVariablesSQL = "SELECT DISTINCT variableName FROM Variable;";
    executeAndProcessSQL(getVariablesSQL,results);
}
void Database::getVariablesPattern(vector<string>& results,string lhsArgs ,string rhsArgs, bool isSubexpression, Query query) {
    string getVariablesPatternSQL;
    if(isSubexpression){
        getVariablesPatternSQL = "select DISTINCT LHSExpression from Pattern where RHSExpression like '%"
                                        + rhsArgs + "%';";
    }else{
        if (rhsArgs == "_") {
            getVariablesPatternSQL = "select DISTINCT LHSExpression from Pattern;";
        }
        else
            getVariablesPatternSQL = "select DISTINCT LHSExpression from Pattern where RHSExpression like '"
                    + rhsArgs + "';";
    }
    cout << "getVariablesPatternSQL " << getVariablesPatternSQL << endl;
    executeAndProcessSQL(getVariablesPatternSQL,results);
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
    string getConstantsSQL = "SELECT DISTINCT constantValue FROM Constant;";
    executeAndProcessSQL(getConstantsSQL,results);
}

void Database::insertParentChildRelation(int parentStatementCodeLine, int childStatementCodeLine) {
    string insertSQL = "INSERT INTO ParentChildRelation (parentStatementCodeLine, childStatementCodeLine) VALUES ("
        + to_string(parentStatementCodeLine) + ", "
        + to_string(childStatementCodeLine) + ");";
    sqlite3_exec(dbConnection, insertSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::getParentChildRelations(vector<string>& results) {
    string getSQL = "SELECT parentStatementCodeLine, childStatementCodeLine FROM ParentChildRelation;";
    executeAndProcessSQL(getSQL,results);
}

void Database::insertAncestorRelation(int ancestorStatementCodeLine, int childStatementCodeLine) {
    string insertSQL = "INSERT INTO AncestorRelation (ancestorStatementCodeLine, childStatementCodeLine) VALUES ("
                       + to_string(ancestorStatementCodeLine) + ", "
                       + to_string(childStatementCodeLine) + ");";
    sqlite3_exec(dbConnection, insertSQL.c_str(), NULL, 0, &errorMessage);
}


void Database::insertModifies(int statementCodeLine, const string& variableName) {
    string insertSQL = "INSERT INTO Modifies (statementCodeLine, variableName) VALUES ("
        + to_string(statementCodeLine) + ", '"
        + variableName + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::getModifies(vector<string>& results) {
    string getSQL = "SELECT statementCodeLine, variableName FROM Modifies;";
    executeAndProcessSQL(getSQL,results);
}

void Database::insertPattern(int statementCodeLine, const string& LHSExpression, const string& RHSExpression) {
    string insertSQL = "INSERT INTO Pattern (statementCodeLine, LHSExpression, RHSExpression) VALUES ("
        + to_string(statementCodeLine) + ", '"
        + LHSExpression + "', '"
        + RHSExpression + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::getPatterns(vector<string>& results) {
    string getSQL = "SELECT statementCodeLine, LHSExpression, RHSExpression FROM Pattern;";
    executeAndProcessSQL(getSQL,results);
}

void Database::insertUses(int statementCodeLine, const string& variableName) {
    string insertSQL = "INSERT INTO Uses (statementCodeLine, variableName) VALUES ("
                       + to_string(statementCodeLine) + ", '"
                       + variableName + "');";
    sqlite3_exec(dbConnection, insertSQL.c_str(), NULL, 0, &errorMessage);
}


void Database::insertCalls(const string& caller, const string& callee) {
    string insertCallSQL = "INSERT INTO Call (procedureCaller, procedureCallee) VALUES ('"
                           + caller + "', '"
                           + callee + "');";
    sqlite3_exec(dbConnection, insertCallSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::insertCallsT(const string& caller, const string& callee) {
    string insertCallSQL = "INSERT INTO CallT (procedureCaller, procedureCallee) VALUES ('"
                           + caller + "', '"
                           + callee + "');";
    sqlite3_exec(dbConnection, insertCallSQL.c_str(), NULL, 0, &errorMessage);
}


void Database::getUses_OutputVar(string leftArg, vector<string>& results, Query queryToExecute) {
    string getUses_OutputVar;
    string type = queryToExecute.declaredVariables[leftArg];
    if(type =="procedure"){
        getUses_OutputVar = "SELECT DISTINCT variableName FROM Uses;";
    }
    else if(type == "while" || type == "if" || type == "read" || type == "assign"){
        getUses_OutputVar = "select DISTINCT variableName\n"
                                   "FROM Uses WHERE statementCodeLine in (\n"
                                   "SELECT statementCodeLine from Uses\n"
                                   "INTERSECT\n"
                                   "SELECT codeLine from Statement WHERE statementType = '"
                                   + type + "');";
    }
    else{
        getUses_OutputVar = "SELECT variableName FROM Uses WHERE statementCodeLine ='"
                            + leftArg + "';";
    }
    cout << "getUses_OutputVar " << getUses_OutputVar <<endl;
    executeAndProcessSQL(getUses_OutputVar,results);
}

void Database::getUses_OutputStmt(string leftArg, string rightArg, vector<string>& results, Query queryToExecute) {
    string getUses_OutputStmt;
    string leftType = queryToExecute.declaredVariables[leftArg];
    string rightType = queryToExecute.declaredVariables[rightArg];
    if(leftType == "stmt" && rightType == ""){
        getUses_OutputStmt = "SELECT DISTINCT statementCodeLine FROM Uses WHERE variableName ='"
                             + rightArg + "';";
    }
    else if(leftType =="stmt" || leftArg == "_"){
        getUses_OutputStmt = "SELECT DISTINCT statementCodeLine FROM Uses;";
    }
    else{
        getUses_OutputStmt = "SELECT DISTINCT statementCodeLine FROM Uses WHERE statementCodeLine ='"
                            + leftArg + "';";
    }
    executeAndProcessSQL(getUses_OutputStmt,results);
}
void Database::getUses_OutputType(string leftArg, string rightArg, vector<string>& results, Query queryToExecute) {
    string getUses_OutputAssign;
    string leftType = queryToExecute.declaredVariables[leftArg];
    string rightType = queryToExecute.declaredVariables[rightArg];
    if(rightType == "" && (leftType == "assign" || leftType == "print" || leftArg == "_") ){
        getUses_OutputAssign = "SELECT DISTINCT Uses.statementCodeLine\n"
                               "FROM Uses\n"
                               "JOIN Statement s ON Uses.statementCodeLine = s.codeLine\n"
                               "WHERE s.statementType = '" + leftType +
                               "' AND Uses.variableName = '" + rightArg +"';";
    }
    else if(leftType == "assign" || leftType == "print" || leftArg == "_") {
        getUses_OutputAssign = "SELECT DISTINCT Uses.statementCodeLine\n"
                               "FROM Uses\n"
                               "JOIN Statement s ON Uses.statementCodeLine = s.codeLine\n"
                               "WHERE s.statementType = '" + leftType + "';";
    }
    else {
        getUses_OutputAssign = "SELECT DISTINCT statementCodeLine FROM Uses WHERE statementCodeLine ='"
                             + leftArg + "';";
    }
    cout << "getUses_OutputAssign " << getUses_OutputAssign << endl;
    executeAndProcessSQL(getUses_OutputAssign,results);
}

void Database::getUses_OutputProcedures(string leftArg, vector<string>& results, Query queryToExecute){
    string getUses_OutputVar;
    if(queryToExecute.declaredVariables[leftArg]=="procedure"){
        getUses_OutputVar = "SELECT DISTINCT p.procedureName\n"
                            "FROM Procedure p\n"
                            "JOIN Statement s ON p.procedureName = s.procedureName\n"
                            "JOIN Uses u ON s.codeLine = u.statementCodeLine";
    }else {
        cout << "lhs codeline" << endl;
        getUses_OutputVar = "SELECT DISTINCT S.procedureName FROM Statement S\n"
                            "JOIN Uses U ON U.statementCodeLine = S.codeLine\n"
                            "JOIN Variable V ON U.variableName = V.variableName AND U.statementCodeLine = V.statementCodeLine\n"
                            "WHERE U.statementCodeLine = '"
                            +leftArg+"' AND V.variableName = '"
                            +queryToExecute.conditions[0].rightArg+"'";
    }
    executeAndProcessSQL(getUses_OutputVar,results);
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
    string getModifies_OutputVarSQL;
    string type = queryToExecute.declaredVariables[leftArg];

    if(type =="stmt" || type=="procedure"){
        getModifies_OutputVarSQL ="SELECT distinct variableName FROM Modifies;";
    }
    else if(type == "while" || type == "if" || type == "read" || type == "assign"){
        getModifies_OutputVarSQL = "select DISTINCT variableName\n"
                                   "FROM Modifies WHERE statementCodeLine in (\n"
                                   "SELECT statementCodeLine from Modifies\n"
                                   "INTERSECT\n"
                                   "SELECT codeLine from Statement WHERE statementType = '"
                                   + type + "');";
    }
    else{
        getModifies_OutputVarSQL = "SELECT DISTINCT variableName FROM Modifies WHERE statementCodeLine ='"
                                   + leftArg + "';";
    }
    cout << "getModifies_OutputVarSQL " << getModifies_OutputVarSQL <<endl;
    executeAndProcessSQL(getModifies_OutputVarSQL,results);
}

void Database::getModifies_OutputStmt(string rightArg, vector<string>& results, Query queryToExecute) {

    string getModifies_OutputStmtSQL;
    string type = queryToExecute.declaredVariables[rightArg];
    if(queryToExecute.declaredVariables[rightArg]=="variable" || rightArg == "_"){
        getModifies_OutputStmtSQL = "SELECT statementCodeLine FROM Modifies;";
    }
    else {
        getModifies_OutputStmtSQL = "SELECT statementCodeLine FROM Modifies WHERE variableName = '"
                                           + rightArg + "';";
    }
    executeAndProcessSQL(getModifies_OutputStmtSQL,results);
}

void Database::getModifies_OutputProcedures(string rightArg, vector<string>& results, Query queryToExecute) {
    string getModifies_OutputProceduresSQL;
    string type = queryToExecute.declaredVariables[rightArg];
    if(type=="variable") {
        getModifies_OutputProceduresSQL = "SELECT procedureName FROM procedure";
    }
    else{
        getModifies_OutputProceduresSQL = "SELECT DISTINCT s.procedureName FROM Statement s JOIN Modifies m ON m.statementCodeLine = s.codeLine WHERE m.variableName = '"
                                                 + rightArg + "';";
    }
    executeAndProcessSQL(getModifies_OutputProceduresSQL,results);
}

void Database::getStatements_OutputAssign(vector<string>& results) {

    dbResults.clear();

    string getStatements_OutputAssignSQL = "SELECT codeLine FROM Statement WHERE statementType = 'assign';";
    sqlite3_exec(dbConnection, getStatements_OutputAssignSQL.c_str(), callback, 0, &errorMessage);

    postProcessDbResults(results, 0);
}

void Database::getPattern_OutputStmt(string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& results, Query queryToExecute) {
    string getPattern_OutputStmtSQL;
    cout << "patternLeftArg " << patternLeftArg << " patternRightArg " << patternRightArg << endl;
    string selectType = queryToExecute.selectType;
    if (selectType == "while" || selectType == "if") {
        //Select w pattern a("test002", _)
        if (!patternLeftArg.empty() && patternRightArg == "_")
            getPattern_OutputStmtSQL = "SELECT P.parentStatementCodeLine AS ParentLine FROM ParentChildRelation P JOIN Pattern P1 ON P.childStatementCodeLine = P1.statementCodeLine WHERE P1.LHSExpression = '"
                                       +patternLeftArg+"' UNION SELECT A.ancestorStatementCodeLine AS ParentLine FROM AncestorRelation A JOIN Pattern P1 ON A.childStatementCodeLine = P1.statementCodeLine WHERE P1.LHSExpression = '"
                                       +patternLeftArg+"';";
    }
    else if (patternLeftArg == "_" && patternRightArg == "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern;";
    }
    else if (patternRightArg == "_" && queryToExecute.declaredVariables[patternLeftArg]=="variable") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE LHSExpression IN (SELECT variableName FROM Variable)";
    }
        //Select a pattern a (v, _"x"_)
    else if (isSubexpression && queryToExecute.declaredVariables[patternLeftArg] == "variable") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE RHSExpression LIKE '%"
                                   +patternRightArg+"%' INTERSECT SELECT statementCodeLine FROM Pattern WHERE LHSExpression IN (SELECT variableName FROM Variable);";
    }
    else if (isSubexpression && patternLeftArg != "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE RHSExpression like '%"
            + patternRightArg + "%' AND LHSExpression = '"
            + patternLeftArg + "';";
    }
    else if (isSubexpression && patternLeftArg == "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE RHSExpression like '%"
            + patternRightArg + "%';";
    }
    else if (patternLeftArg == "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE RHSExpression = '"
            + patternRightArg + "';";
    }
    else if (patternRightArg == "_") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE LHSExpression = '"
            + patternLeftArg + "';";
    }
    else if (patternLeftArg != "" && patternRightArg != "") {
        getPattern_OutputStmtSQL = "SELECT statementCodeLine FROM Pattern WHERE LHSExpression = '"
            + patternLeftArg + "' AND RHSExpression = '"
            + patternRightArg + "';";
    }
    executeAndProcessSQL(getPattern_OutputStmtSQL,results);
}

void Database::getModifies_OutputParents(string leftArg, vector<string>& results) {
    string getModifies_OutputParentsSQL = "SELECT statementCodeLine from Modifies\n"
                                          "INTERSECT\n"
                                          "SELECT codeLine from Statement WHERE statementType = '"
                                          + leftArg + "';";
    executeAndProcessSQL(getModifies_OutputParentsSQL,results);
}

void Database::getCombo_Modifies_Pattern_OutputProcedure(string res, vector<string>& results) {
    string getCombo_Modifies_Pattern_OutputProcedureSQL = "SELECT DISTINCT procedureName FROM Statement WHERE codeLine in ("
        + res + ");";
    executeAndProcessSQL(getCombo_Modifies_Pattern_OutputProcedureSQL,results);
}

void Database::getCombo_Modifies_Pattern_OutputAssign(string res, vector<string>& results) {
    string getCombo_Modifies_Pattern_OutputAssignSQL = "SELECT statementCodeLine FROM Modifies WHERE statementCodeLine in ("
                                                       + res + "); ";
    executeAndProcessSQL(getCombo_Modifies_Pattern_OutputAssignSQL,results);
}

void Database::getCombo_Modifies_Pattern_OutputVar(string res, vector<string>& results) {
    string getCombo_Modifies_Pattern_OutputVarSQL = "SELECT variableName FROM Modifies WHERE statementCodeLine in ("
                                                    + res + "); ";
    executeAndProcessSQL(getCombo_Modifies_Pattern_OutputVarSQL,results);
}

void Database::getCalls_OutputProcedures(string leftArg, string rightArg, vector<string>& results, Query queryToExecute){
    string getCalls_OutputProceduresSQL;
    string leftType = queryToExecute.declaredVariables[leftArg];
    string rightType = queryToExecute.declaredVariables[rightArg];
    if(leftType == "procedure" && rightArg == "_"){
        getCalls_OutputProceduresSQL = "SELECT DISTINCT procedureCaller FROM Call;";
    }
    else if(rightType == "procedure" && leftArg == "_"){
        getCalls_OutputProceduresSQL = "SELECT procedureCallee FROM Call;";
    }
    else if(leftType == "procedure" || leftArg == "_"){
        getCalls_OutputProceduresSQL = "SELECT DISTINCT procedureCaller FROM Call WHERE procedureCallee = '"
                                              + rightArg + "'; ";
    }
    else if (rightType== "procedure"|| rightArg == "_" ){
        getCalls_OutputProceduresSQL = "SELECT procedureCallee FROM Call WHERE procedureCaller = '"
                                       + leftArg + "'; ";
    }
    executeAndProcessSQL(getCalls_OutputProceduresSQL,results);
}

void Database::getCallsT_OutputProcedures(string leftArg, string rightArg, vector<string>& results, Query queryToExecute){
    string getCallsT_OutputProceduresSQL;
    string leftType = queryToExecute.declaredVariables[leftArg];
    string rightType = queryToExecute.declaredVariables[rightArg];
    if(leftType == "procedure" && rightArg == "_"){
        cout << "asdf" << endl;
        getCallsT_OutputProceduresSQL = "SELECT DISTINCT procedureCaller FROM CallT";
    }
    else if(rightType == "procedure" && leftArg == "_"){
        getCallsT_OutputProceduresSQL = "SELECT procedureCallee FROM CallT;";
    }
    else if(leftType == "procedure" || leftArg == "_"){
        getCallsT_OutputProceduresSQL = "SELECT procedureCaller FROM CallT WHERE procedureCallee = '"
                                       + rightArg + "'; ";
    }
    else if (rightType== "procedure" || rightArg == "_" ){
        getCallsT_OutputProceduresSQL = "SELECT procedureCallee FROM CallT WHERE procedureCaller = '"
                                       + leftArg + "'; ";
    }
    executeAndProcessSQL(getCallsT_OutputProceduresSQL,results);
}

void Database::getParent(string selectVar, string selectType, string leftArg, string rightArg, vector<string>& results, Query query) {

    dbResults.clear();
    string getParentSQL;

    bool islhsSyn = 0, isrhsSyn = 0;
    string lhsSynType, rhsSynType;
    bool AncestorExists = 0;

    Database::prepareContext_Parent(query.declaredVariables, leftArg, rightArg, islhsSyn, lhsSynType, isrhsSyn, rhsSynType, AncestorExists);




    if (leftArg == "_") {
        cout << "rhs line number" << endl;
        string getParent = "SELECT parentStatementCodeLine FROM ParentChildRelation WHERE childStatementCodeLine = '"
                           + rightArg + "';";
        sqlite3_exec(dbConnection, getParent.c_str(), callback, 0, &errorMessage);
        Parent_HelperFunctions::handleWildcardLHS(getParentSQL, selectVar, selectType, lhsSynType, rhsSynType, leftArg, rightArg, isrhsSyn, results, dbResults);

    }
    else if (islhsSyn) {
        Parent_HelperFunctions::handleSynonymLHS(getParentSQL, selectVar, selectType, lhsSynType, rhsSynType, leftArg,
                                                 rightArg, isrhsSyn, results, dbResults);
    }
    else {
        cout << "rhs lineNumber" << endl;
        string getParent = "SELECT parentStatementCodeLine FROM ParentChildRelation WHERE parentStatementCodeLine = '"
                           + leftArg + "' AND childStatementCodeLine = '"
                           + rightArg + "';";
        sqlite3_exec(dbConnection, getParent.c_str(), callback, 0, &errorMessage);

        Parent_HelperFunctions::handleLineNumberLHS(getParentSQL, selectVar, selectType, lhsSynType, rhsSynType, leftArg, rightArg, isrhsSyn, results, dbResults);
    }
    executeAndProcessSQL(getParentSQL,results);
}

void Database::getParentT(string selectVar, string selectType, string leftArg, string rightArg, vector<string>& results, Query query) {
    dbResults.clear();

    string getParentSQL;
    bool islhsSyn = 0, isrhsSyn = 0;
    string lhsSynType, rhsSynType;
    bool AncestorExists = 0;

    Database::prepareContext_Parent(query.declaredVariables, leftArg, rightArg, islhsSyn, lhsSynType, isrhsSyn, rhsSynType, AncestorExists);

    if (leftArg == "_") {
        string getGrandParent = "SELECT ancestorStatementCodeLine FROM AncestorRelation WHERE childStatementCodeLine = '"
                                + rightArg + "';";
        sqlite3_exec(dbConnection, getGrandParent.c_str(), callback, 0, &errorMessage);
        ParentT_HelperFunctions::handleWildcardLHS(getParentSQL,selectVar, selectType, lhsSynType, rhsSynType, leftArg, rightArg, isrhsSyn, AncestorExists, results, dbResults);
    }
    else if (islhsSyn) {
        ParentT_HelperFunctions::handleSynonymLHS(getParentSQL,selectVar, selectType, lhsSynType, rhsSynType, leftArg, rightArg, isrhsSyn, AncestorExists, results, dbResults);
    }

    else {
        string getGrandParent = "SELECT ancestorStatementCodeLine FROM AncestorRelation WHERE ancestorStatementCodeLine = '"
                                + leftArg + "' AND childStatementCodeLine = '"
                                + rightArg + "';";
        sqlite3_exec(dbConnection, getGrandParent.c_str(), callback, 0, &errorMessage);
        ParentT_HelperFunctions::handleLineNumberLHS(getParentSQL, selectVar, selectType, lhsSynType, rhsSynType, leftArg, rightArg, isrhsSyn, AncestorExists, results, dbResults);
    }
    executeAndProcessSQL(getParentSQL,results);
}
void Database::prepareContext_Parent(const std::unordered_map<std::string, std::string>& declaredVariables,
                                     const std::string& leftArg, const std::string& rightArg,
                                     bool& isLhsSynonym, std::string& lhsSynType,
                                     bool& isRhsSynonym, std::string& rhsSynType,
                                     bool& ancestorExists) {


    cout << "Starting prepareContext function..." << endl;

    isLhsSynonym = declaredVariables.count(leftArg) > 0;
    if (isLhsSynonym) {
        lhsSynType = declaredVariables.at(leftArg);
        std::cout << "LHS Synonym found: " << lhsSynType << std::endl;
    }
    else {
        std::cout << "No LHS Synonym for argument: " << leftArg << std::endl;
    }

    isRhsSynonym = declaredVariables.count(rightArg) > 0;
    if (isRhsSynonym) {
        rhsSynType = declaredVariables.at(rightArg);
        std::cout << "RHS Synonym found: " << rhsSynType << std::endl;
    }
    else {
        std::cout << "No RHS Synonym for argument: " << rightArg << std::endl;
    }
    std::cout << "Executing SQL to check for ancestor existence..." << std::endl;
    std::string getAncestor = "SELECT * FROM AncestorRelation;";
    sqlite3_exec(dbConnection, getAncestor.c_str(), callback, 0, &errorMessage);

    ancestorExists = !dbResults.empty();
    std::cout << "Ancestor relation exists: " << (ancestorExists ? "Yes" : "No") << std::endl;
    dbResults.clear();

    std::cout << "Finished prepareContext function." << std::endl;}

bool Database::executeCheckQuery(string sqlQuery, vector<string> params) {
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(dbConnection, sqlQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(dbConnection) << std::endl;
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        if (sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
            std::cerr << "Failed to bind parameters: " << sqlite3_errmsg(dbConnection) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }
    }

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = true;
    }
    sqlite3_finalize(stmt);
    return exists;
}

bool Database::checkCallsRelationship(string caller, string callee) {
    std::string sql = "SELECT 1 FROM Call WHERE procedureCaller = ? AND procedureCallee = ?";
    return executeCheckQuery(sql, {caller, callee});
}
bool Database::checkCallsTRelationship(string caller, string callee) {
    std::string sql = "SELECT 1 FROM CallT WHERE procedureCaller = ? AND procedureCallee = ?";
    return executeCheckQuery(sql, {caller, callee});
}

bool Database::checkParentTRelationship(string parent, string child) {
    std::string sql = "SELECT 1 FROM AncestorRelation WHERE ancestorStatementCodeLine = ? AND childStatementCodeLine = ?";
    return executeCheckQuery(sql, {parent, child}) || checkParentRelationship(parent, child);
}

bool Database::checkParentRelationship(string parent, string child) {
    std::string sql = "SELECT 1 FROM ParentChildRelation WHERE parentStatementCodeLine = ? AND childStatementCodeLine = ?";
    return executeCheckQuery(sql, {parent, child});
}

bool Database::checkModifiesRelationship(string statementCodeLine, string variableName, string statementType) {
    std::string sqlQuery = R"(
        SELECT EXISTS(
            SELECT 1 FROM Modifies m
            JOIN Statement s ON m.statementCodeLine = s.codeLine
            WHERE m.statementCodeLine = ? AND m.variableName = ?
    )";

    vector<string> params = {statementCodeLine, variableName};

    if (statementType!="") {
        sqlQuery += " AND s.statementType = ?";
        params.push_back(statementType);
    }

    sqlQuery += ")";

    return executeCheckQuery(sqlQuery, params);
}



