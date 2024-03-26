#pragma once
#include <iostream>
#include <string>
#include "StatementInfo.h"
#include "VariableProcessing.h"
#include "../db/Database.h"


class ModifiesProcessing {
public:
    static void processModifies(vector<StatementInfo>& statementInfo);
};

