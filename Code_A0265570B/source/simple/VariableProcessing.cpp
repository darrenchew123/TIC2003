#include "VariableProcessing.h"

// Process variable and insert into variable table
void VariableProcessing::processVariable(const string& varName, const int& lineCount){
    if(!SyntaxValidator::checkName(varName)) return;
    Database::insertVariable(varName,lineCount);
}

// Extract variable
string VariableProcessing::extractVariableName(const string& statement, const string& statementType) {
    istringstream iss(statement);
    string word;
    if (statementType == "read") {
        iss >> word >> word;
        return word;
    } else if (statementType == "assign") {
        getline(iss, word, '=');
        word.erase(remove_if(word.begin(), word.end(), ::isspace), word.end());
        return word;
    }
    return "";
}