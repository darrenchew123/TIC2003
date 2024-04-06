#pragma once

//#ifndef STATEMENT_INFO_H
//#define STATEMENT_INFO_H
#include <string>

struct StatementInfo {
    int lineCount;
    std::string statementContent;
    std::string statementType;
    std::string procedureName;
};

//#endif // STATEMENT_INFO_H
