
#include "UsesProcessing.h"

void UsesProcessing::processUses(vector<StatementInfo>& statementInfo) {
    for (const auto& info : statementInfo) {
        vector<string> usedVariables;
        if (info.statementType == "assign") {
            usedVariables = extractAssignmentVariables(info.statementContent);
        } else if (info.statementType == "print") {
            usedVariables = extractPrintVariables(info.statementContent);
        }

        for (const auto& var : usedVariables) {
            Database::insertUses(info.lineCount, var);
        }
    }
}

vector<string> UsesProcessing::extractPrintVariables(const string& statement) {
    istringstream iss(statement);
    string word, variable;
    iss >> word;
    iss >> variable;

    return variable.empty() ? vector<string>{} : vector<string>{variable};
}

vector<string> UsesProcessing::extractAssignmentVariables(const string& statement) {
    string assignmentExpr = statement.substr(statement.find('=') + 1);
    istringstream iss(assignmentExpr);
    string token;
    vector<string> variables;
    while (iss >> token) {
        if (isalpha(token[0])) {
            variables.push_back(token);
        }
    }
    return variables;
}