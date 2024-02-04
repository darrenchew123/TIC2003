#include "SourceProcessor.h"

void SourceProcessor::processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens){
    inProcedure = true;
    procedureName = tokens[++i];
    Database::insertProcedure(procedureName);
}

void SourceProcessor::processFactor(const string& procedureName, const string& varName, const string& factor, int& lineCount) {
    if (isdigit(factor[0])) {
        int constantValue = stoi(factor);
        Database::insertConstant(lineCount,constantValue);
        Database::insertVariable(varName,lineCount);
        Database::insertStatement(procedureName, "assign", varName + "=" + factor, lineCount);
    } else {
        Database::insertStatement(procedureName, "assign", varName + "=" + factor, lineCount);
    }
}


void SourceProcessor::processOtherStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens) {
    string varName = tokens[++i];
    Database::insertVariable(varName,lineCount);
    Database::insertStatement(procedureName, token, varName, lineCount);
    i++; // Skip the semicolon
}

void SourceProcessor::processAssignmentStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens) {
    string varName = token;
    i += 2; // Skip '=' and move to the factor
    string factor = tokens[i];
    processFactor(procedureName, varName, factor, lineCount);
    i++; // Skip the semicolon
}

void SourceProcessor::processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens) {
    if (token == "{") {
        return;
    } else if (token == "\n" && tokens.at(i-1)!="\n") {
        lineCount++;
    } else if (token == "read" || token == "print") {
        processOtherStatement(procedureName, token, i, lineCount, tokens);
    } else if (token != "=" && token != ";") {
        processAssignmentStatement(procedureName, token, i, lineCount, tokens);
    }
}

void SourceProcessor::process(string &program) {
    // Initialize the database
    Database::initialize();

    // Tokenize the program
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(program, tokens);
    for (const auto& token : tokens) {
        std::cout <<"tokens "<< token << endl;
    }

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