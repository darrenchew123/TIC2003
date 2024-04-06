#include "VariableProcessing.h"

// Process variable and insert into variable table
void VariableProcessing::processVariable(const string& varName, const int& lineCount){
    if(!SyntaxValidator::checkName(varName)) return;
    Database::insertVariable(varName,lineCount);
}

