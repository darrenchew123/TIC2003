#include "SourceProcessor.h"

// Utility methods
void SourceProcessor::incrementLineCount(int& lineCount) {
    lineCount++;
}

void SourceProcessor::handleProcedure(const string& token, size_t& i, string& procedureName, bool& inProcedure) {
    inProcedure = true;
    procedureName = token;
    Database::insertProcedure(procedureName);
}

void SourceProcessor::handleStatement(const string& procedureName, const string& token, size_t& i, int lineCount, vector<string>& tokens) {
    if (token == "read" || token == "print") {
        string varName = tokens[++i];
        Database::insertStatement(procedureName, token, varName, lineCount);
    } else {
        // Handle assignment
        handleAssignment(procedureName, token, i, lineCount, tokens);
    }
    // Skip the semicolon
    i++;
}

void SourceProcessor::handleAssignment(const string& procedureName, const string& varName, size_t& i, int lineCount, vector<string>& tokens) {
    string factor = tokens[i+2]; // Assuming '=' is at i+1
    if (isdigit(factor[0])) {
        // Factor is a constant
        int constantValue = stoi(factor);
        Database::insertConstant(constantValue);
        Database::insertStatement(procedureName, "assign", varName + "=" + factor, lineCount);
    } else {
        // Factor is a variable
        Database::insertStatement(procedureName, "assign", varName + "=" + factor, lineCount);
    }
    // Skip '=' and factor
    i += 2;
}

// Main processing method
void SourceProcessor::process(string program) {
    Database::initialize();
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(program, tokens);

    int lineCount = 1;
    bool inProcedure = false;
    string procedureName;

    for (size_t i = 0; i < tokens.size(); i++) {
        string token = tokens[i];

        if (token == "procedure") {
            handleProcedure(tokens[++i], i, procedureName, inProcedure);
        }
        else if (token == "}") {
            inProcedure = false;
        }
        else if (inProcedure) {
            if (token == "{") continue;
            else if (token == "\n") incrementLineCount(lineCount);
            else if (token == "read" || token == "print" || (token != "=" && token != ";")) {
                handleStatement(procedureName, token, i, lineCount, tokens);
            }
        }
    }
}
