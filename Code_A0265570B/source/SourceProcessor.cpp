#include "SourceProcessor.h"

//Process logic for procedure
void SourceProcessor::processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens){
    inProcedure = true;
    procedureName = tokens[++i];
    Database::insertProcedure(procedureName);
}

//Process constant logic + assignmnet
void SourceProcessor::processConstantAssignment(const string& procedureName, const string& varName, const string& constantString, int& lineCount) {
    if (isdigit(constantString[0])) {
        int constantValue = stoi(constantString);
        Database::insertConstant(lineCount,constantValue);
        Database::insertVariable(varName,lineCount);
        Database::insertStatement(procedureName, "assign", varName + "=" + constantString, lineCount);
    } else {
        Database::insertStatement(procedureName, "assign", varName + "=" + constantString, lineCount);
    }
}

//Process other statement which
void SourceProcessor::processOtherStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens) {
    string varName = tokens[++i];
    Database::insertVariable(varName,lineCount);
    Database::insertStatement(procedureName, token, varName, lineCount);
    //Skip the semicolon
    i++;
}

//Process assignment logic which includes constant
void SourceProcessor::processAssignmentStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens) {
    string varName = token;
    // Skip = and move to the constant factor
    i += 2;
    string constantString = tokens[i];
    processConstantAssignment(procedureName, varName, constantString, lineCount);
    // Skip the semicolon
    i++;
}

//In procedure logic, logic that is not in procedure will not be excuted here
void SourceProcessor::processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens) {
    if (token == "{" || (token == "\n" && tokens.at(i-1)=="\n")) {
        return;
    }else if (token == "\n") {
        lineCount++;
    } else if (token == "read" || token == "print") {
        processOtherStatement(procedureName, token, i, lineCount, tokens);
    } else if (token != "=" && token != ";") {
        processAssignmentStatement(procedureName, token, i, lineCount, tokens);
    }
}

//Parse source program
void SourceProcessor::process(string &program) {
    // Initialize the database
    Database::initialize();

    // Tokenize the program
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(program, tokens);

    int lineCount = 0;
    bool inProcedure = false;
    string procedureName;

    for (int i = 0; i < tokens.size(); i++) {
        string token = tokens[i];

        if (token == "procedure") {
            processProcedure(inProcedure, procedureName, i, tokens);
        } else if (token == "}") {
            inProcedure = false;
        } else if (inProcedure) {
            processInProcedure(token, procedureName, i, lineCount, tokens);
        }
    }
}