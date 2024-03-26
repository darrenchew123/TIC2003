#include "ModifiesProcessing.h"

void ModifiesProcessing::processModifies(vector<StatementInfo>& statementInfo) {
    for (const auto& info : statementInfo) {
        string variableName = VariableProcessing::extractVariableName(info.statementContent, info.statementType);
        if (!variableName.empty()) {
            Database::insertModifies(info.lineCount, variableName);
        }
    }
}