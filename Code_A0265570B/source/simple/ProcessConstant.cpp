#include "ProcessConstant.h"

//Process constant and insert into constant table
void ProcessConstant::processConstant(const string& constantString, int& lineCount) {
    int constantValue = stoi(constantString);
    Database::insertConstant(lineCount, constantValue);
}
