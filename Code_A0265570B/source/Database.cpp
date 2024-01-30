#include "Database.h"

sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;

// method to connect to the database and initialize tables in the database
void Database::initialize() {
    // open a database connection and store the pointer into dbConnection
    sqlite3_open("database.db", &dbConnection);

    // drop existing tables (if any)
    const char* dropTablesSQL = "DROP TABLE IF EXISTS Variable;"
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
                                          "codeLine INT,"
                                          "procedureName VARCHAR(255),"
                                          "statementType VARCHAR(50),"
                                          "statementContent VARCHAR(255),"
                                          "PRIMARY KEY (codeLine, procedureName),"
                                          "FOREIGN KEY (procedureName) REFERENCES Procedure(procedureName));";
    sqlite3_exec(dbConnection, createStatementTableSQL, NULL, 0, &errorMessage);

    // create Variable table
    const char* createVariableTableSQL = "CREATE TABLE Variable ("
                                         "variableName INT PRIMARY KEY,"
                                         "codeLine VARCHAR(50),"
                                         "FOREIGN KEY (codeLine) REFERENCES Statement(codeLine));";
    sqlite3_exec(dbConnection, createVariableTableSQL, NULL, 0, &errorMessage);

    // create the Constant table
    const char* createConstantTableSQL = "CREATE TABLE Constant ("
                                         "constantId INT PRIMARY KEY,"
                                         "constantValue INT);";
    sqlite3_exec(dbConnection, createConstantTableSQL, NULL, 0, &errorMessage);

    // initialize the result vector
    dbResults = vector<vector<string> >();
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
    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
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
