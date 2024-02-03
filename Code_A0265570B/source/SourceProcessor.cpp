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
    if (token == "read" || token == "print" || token == "call") {
        string varName = tokens[++i];
        Database::insertStatement(procedureName, token, varName, lineCount);
    } else {
        // Handle assignment
        handleAssignment(procedureName, token, i, lineCount, tokens);
    }

    // Skip the semicolon, open curly bracket
    i++;
}

void SourceProcessor::handleAssignment(const string& procedureName, const string& varName, size_t& i, int lineCount, vector<string>& tokens) {
    /*
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
    */

    string statementContent = varName;
    i++;

    // create statementContent until token reaches ";"
    while (tokens[i] != ";") {
        statementContent += tokens[i];
        i++;
    }
    
    Database::insertStatement(procedureName, "assign", statementContent, lineCount);
    i++;

    // Skip '=' and factor
    //i += 2;
    
}

// Main processing method
void SourceProcessor::process(string program) {
    Database::initialize();
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(program, tokens);

    int lineCount = 0; 

    bool isProcedureLine = false;
    bool inProcedure = false;

    bool inIfThenLine = false;
    bool inIfThen = false;

    bool inWhileLine = false;
    bool inWhile = false;
    
    string procedureName;

    for (size_t i = 0; i < tokens.size(); i++) {
        string token = tokens[i];

        if (token == "procedure") {
            isProcedureLine = true;
            handleProcedure(tokens[++i], i, procedureName, inProcedure); // inProcedure = true after calling fn
        }
        else if (token == "}") {
            if (inIfThen == false && inWhile == false)
                inProcedure = false;
            else if (inIfThen)
                inIfThen = false;
            else if (inWhile)
                inWhile = false;
        }
        else if (inProcedure) {
            
            if (inIfThenLine) { //i++ until token "then" is reach}
                if (token == "then") 
                    inIfThenLine = false;
                else 
                    continue;
            }

            else if (token == "if") { //i++ until token "then" is reach}
                inIfThenLine = true;
                inIfThen = true;
                continue;
            }

            else if (token == "{") continue;

            else if (token == "\n") {
                /*
                if (isProcedureLine) { //to exclude procedure line in line counter
                    isProcedureLine = false;
                }
                else {
                    incrementLineCount(lineCount);
                }
                */
                incrementLineCount(lineCount);
            }
            else if (token == "else") continue;
            
            //else if (token == "read" || token == "print" || token == "call" || (token != "=" && token != ";")) {
            else if (token == "read" || token == "print" || token == "call" || (token != ";")) {
                handleStatement(procedureName, token, i, lineCount, tokens);
            }
        }
    }
}
