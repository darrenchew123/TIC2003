#include "SourceProcessor.h"

// method for processing the source program
// This method currently only inserts the procedure name into the database
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all the required syntax.
void SourceProcessor::process(string program) {
    // Initialize the database
    Database::initialize();

    // Tokenize the program
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(program, tokens);

    int lineCount = 1;
    bool inProcedure = false;
    string procedureName;



    for (size_t i = 0; i < tokens.size(); i++) {
        string token = tokens[i];

        if (token == "procedure") {
            inProcedure = true;
            procedureName = tokens[++i];
            Database::insertProcedure(procedureName);
        }
        else if (token == "}") {
            inProcedure = false;
        }
        else if (inProcedure) {
            if (token == "{") {
                continue;
            }
            else if (token == "\n") {
                lineCount++;
            }
            else if (token == "read" || token == "print") {
                string varName = tokens[++i];
                Database::insertStatement(procedureName, token, varName, lineCount);
                i++; // Skip the semicolon
            } else if (token != "=" && token != ";") {
                // Assume token is a variable name for an assign statement
                string varName = token;
                i += 2; // Skip '=' and move to the factor

                string factor = tokens[i];
                if (isdigit(factor[0])) {
                    // Factor is a constant
                    int constantValue = stoi(factor);
                    Database::insertConstant(constantValue);
                    Database::insertStatement(procedureName, "assign", varName + "=" + factor, lineCount);
                } else {
                    // Factor is a variable
                    Database::insertStatement(procedureName, "assign", varName + "=" + factor, lineCount);
                }
                i++; // Skip the semicolon
            }
        }
    }
}