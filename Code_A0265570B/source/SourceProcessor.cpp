#include "SourceProcessor.h"

//Process logic for procedure

// Check if it's integer to process constant
bool SourceProcessor::isInteger(const string& intString) {
    if (intString.empty()) return false;
    int startIndex = 0;
    // return false if not all digits
    return all_of(intString.begin() + startIndex, intString.end(), ::isdigit);
}

// Process procedure
void SourceProcessor::processProcedure(bool &inProcedure, string &procedureName, int &i, const vector<string> &tokens){
    inProcedure = true;
    procedureName = tokens[++i];
    Database::insertProcedure(procedureName);
}

// Process statement and insert into statement table
void SourceProcessor::processStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes) {
    if(statementTypes.size() == 0) return;
    string statementContent;
    int counter = i - 1;
    while(counter>=0 && tokens[counter]!= "\n"){
        statementContent = tokens[counter] + " " + statementContent;
        counter--;
    }
    Database::insertStatement(procedureName, statementTypes.top(), statementContent, lineCount);
    statementTypes.pop();
}

// Process variable and insert into variable table
void SourceProcessor::processVariable(const string& varName, const int& lineCount){
    Database::insertVariable(varName,lineCount);
}

//Process constant and insert into constant table
void SourceProcessor::processConstant(const string& constantString, int& lineCount) {
    int constantValue = stoi(constantString);
    Database::insertConstant(lineCount, constantValue);
}

//Process read print assignment logic and add into statement type
void SourceProcessor::processReadPrintAssignment(const string& token, stack<string>& statementTypes) {
    if(token == "=") statementTypes.push("assign");
    else statementTypes.push(token);
}


//In procedure logic, logic that is not in procedure will not be excuted here
void SourceProcessor::processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes) {
    if (token == "{" || token == ";" || token == "+" || token == "-" || token == "*" || token == "/" || (token == "\n" && tokens.at(i-1)=="\n")) {
        return;
    } else if (token == "read" || token == "print" || token == "=") {
        processReadPrintAssignment(token, statementTypes);
    } else if (isInteger(token)) {
        processConstant(token, lineCount);
    }else if (token == "\n") {
        processStatement(procedureName, token, i, lineCount, tokens, statementTypes);
        lineCount++;
    } else {
        processVariable(token, lineCount);
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
    stack<string> statementTypes;

    for(auto token : tokens){
        cout <<"Token :"<<token <<endl;
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
            processInProcedure(token, procedureName, i, lineCount, tokens, statementTypes);
        }
    }
}