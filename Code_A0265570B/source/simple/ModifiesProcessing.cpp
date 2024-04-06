#include "ModifiesProcessing.h"

void ModifiesProcessing::processModifies(vector<StatementInfo>& statementInfo) {
    for (const auto& info : statementInfo) {
        string variableName = extractModifiesVariable(info.statementContent, info.statementType);
        if (!variableName.empty()) {
            Database::insertModifies(info.lineCount, variableName);
        }
    }
}

// Extract variable
string ModifiesProcessing::extractModifiesVariable(const string& statement, const string& statementType) {
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