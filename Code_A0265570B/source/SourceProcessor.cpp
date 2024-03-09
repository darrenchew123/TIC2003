#include "SourceProcessor.h"

bool SourceProcessor::checkName(string token){
    string namePattern = "[a-zA-Z][a-zA-Z0-9]*";
    return regex_match(token, regex(namePattern));
}


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
    try {
        SourceProcessor::checkName(procedureName);
    }
    catch (const std::exception& e) {
        std::cerr << "Procedure name does not adhere to naming convention: " << e.what() << std::endl;
        throw std::runtime_error("Procedure name error");
    }

    Database::insertProcedure(procedureName);
}

// Process statement and insert into statement table
void SourceProcessor::processStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, vector<StatementInfo> &statementInfo) {
    if(statementTypes.size() == 0) return;
    string statementContent;
    int counter = i - 2;
    while(counter>=0 && tokens[counter]!= "\n"){
        statementContent = tokens[counter] + " " + statementContent;
        counter--;
    }
    statementInfo.push_back({lineCount, statementContent, statementTypes.top()});
    Database::insertStatement(procedureName, statementTypes.top(), statementContent, lineCount);
    statementTypes.pop();
    // If there is a parent statement, insert relation
    if (!parentStack.empty() && parentStack.top()!=lineCount) {
        int parentLine = parentStack.top();
        Database::insertParentChildRelation(parentLine, lineCount);
    }

}

// Process variable and insert into variable table
void SourceProcessor::processVariable(const string& varName, const int& lineCount){
    if(!checkName(varName)) return;
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

void SourceProcessor::processControlFlow(const string& token, stack<string>& statementTypes, stack<int>& parentStack, int& lineCount, stack<int>& ifStack) {
    if (token == "if" || token == "while" ) {
        statementTypes.push(token);
        parentStack.push(lineCount);
        if (token == "if") {
            ifStack.push(lineCount);
        }
    }
    else if (token == "else") {
        if (ifStack.empty()) {
            throw std::runtime_error("Syntax error: 'else' without preceding 'if'");
        }
        parentStack.push(ifStack.top());
        ifStack.pop();
    }

    else if (token == "}" && !parentStack.empty()){
        parentStack.pop();
    }
}

//In procedure logic, logic that is not in procedure will not be excuted here
void SourceProcessor::processInProcedure(const string& token, const string& procedureName, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, stack<bool>& expressionStack, vector<StatementInfo> &statementInfo, stack<int>& ifStack) {
    if (token == "{" || token == ";" || token == "+" || token == "-" || token == "*" || token == "/" ||(token == "then") ||(token == "\n" && tokens.at(i-1) == "\n") || (token == "\n" && tokens.at(i-1) == "{" && tokens.at(i-2) == "else")) {
        return;
    } else if (token == "read" || token == "print" || token == "=") {
        if (expressionStack.empty() || !expressionStack.top()) { // Only process as read/print/assign if not in an expression
            processReadPrintAssignment(token, statementTypes);
        }
    } else if (isInteger(token)) {
        processConstant(token, lineCount);
    } else if (token == "\n") {
        processStatement(procedureName, token, i, lineCount, tokens, statementTypes,parentStack, statementInfo);
        lineCount++;
    } else if (token == "if" || token == "else" || token == "while") {
        processControlFlow(token, statementTypes, parentStack, lineCount, ifStack);
    } else if (token == "(") {
        expressionStack.push(true);
    } else if (token == ")" && !expressionStack.empty()) {
        expressionStack.pop();
    } else {
        processVariable(token, lineCount);
    }
}

void SourceProcessor::processExpression(std::vector<StatementInfo> &statementInfo) {
    for (const auto& info : statementInfo) {
        if(info.statementType != "assign") continue;
        size_t equalsPosition = info.statementContent.find('=');
        if (equalsPosition != std::string::npos) {
            string lhs = info.statementContent.substr(0, equalsPosition);
            string rhs = info.statementContent.substr(equalsPosition + 1);

            rhs = InfixToPostfix::infixToPostfix(rhs);

            Database::insertPattern(info.lineCount, lhs, rhs);
        } else {
            std::cerr << "Assignment operator not found in statement: " << info.statementContent << std::endl;
        }
    }
}

std::string SourceProcessor::extractVariableName(const std::string& statement, const std::string& statementType) {
    std::istringstream iss(statement);
    std::string word;
    if (statementType == "read") {
        iss >> word >> word;
        return word;
    } else if (statementType == "assign") {
        std::getline(iss, word, '=');
        word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
        return word;
    }
    return "";
}

void SourceProcessor::processModifies(std::vector<StatementInfo>& statementInfo) {
    for (const auto& info : statementInfo) {
        std::string variableName = extractVariableName(info.statementContent, info.statementType);
        if (!variableName.empty()) {
            Database::insertModifies(info.lineCount, variableName);
        }
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
    stack<bool> expressionStack;
    stack<int> parentStack, ifStack;
    string lhs, rhs;
    vector<StatementInfo> statementInfo;

    for(auto token : tokens){
        cout <<"Token :"<<token <<endl;
    }

    int lineCount = 0;
    int blockDepth = 0;
    bool inProcedure = false;
    string procedureName;

    for (int i = 0; i < tokens.size(); i++) {
        string token = tokens[i];

        if (token == "procedure") {
            blockDepth = 0;
            processProcedure(inProcedure, procedureName, i, tokens);
            inProcedure = true;
        } else if (token == "{" && inProcedure) {
            blockDepth++;
        } else if (token == "}" && inProcedure) {
            if (blockDepth > 0) {
                blockDepth--;
                if(!parentStack.empty()) parentStack.pop();
            } else {
                inProcedure = false;
            }
        } else if (inProcedure) {
            processInProcedure(token, procedureName, i, lineCount, tokens, statementTypes, parentStack,expressionStack, statementInfo, ifStack);
        }
    }

    processExpression(statementInfo);
    processModifies(statementInfo);
}