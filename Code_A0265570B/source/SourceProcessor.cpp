#include "SourceProcessor.h"

void SourceProcessor:: handleProcedure(const string& token, int& i, string& procedureName, bool& inProcedure) {
    inProcedure = true;
    procedureName = token;
    Database::insertProcedure(procedureName);
}

void SourceProcessor:: handleStatement(const string& procedureName, const string& token, int& i, int lineCount, vector<string>& tokens) {
    if (token == "read" || token == "print" || token == ) {
        string varName = tokens[++i];
        Database::insertStatement(procedureName, token, varName, lineCount);
    } else {
        handleAssignment(procedureName, token, i, lineCount, tokens);
    }
    i++;
}

void SourceProcessor:: handleAssignment(const string& procedureName, const string& varName, int& i, int lineCount, vector<string>& tokens) {
    string statementContent = varName + "=";
    i++; // Move past variable name to '='

    // Create statementContent until token reaches ";"
    while (tokens[i] != ";") {
        statementContent += tokens[i];
        i++;
    }

    Database::insertStatement(procedureName, "assign", statementContent, lineCount);
    i++; // Adjust for next token processing
}

void SourceProcessor:: process(string& program) {
    Database::initialize();
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(program, tokens);

    int lineCount = 1;
    bool inProcedure = false;
    bool inIfThen = false;
    bool inWhile = false;
    string procedureName;

    for (int i = 0; i < tokens.size(); i++) {
        string token = tokens[i];

        if (token == "procedure") {
            handleProcedure(tokens[++i], i, procedureName, inProcedure);
        } else if (token == "}") {
            if (inIfThen || inWhile) {
                inIfThen = false;
                inWhile = false;
            } else {
                inProcedure = false;
            }
        } else if (inProcedure) {
            if (token == "if") {
                inIfThen = true;
            } else if (token == "\n") {
                lineCount++;
            } else if (token != ";" && token != "{") {
                handleStatement(procedureName, token, i, lineCount, tokens);
            }
        }
    }
}
