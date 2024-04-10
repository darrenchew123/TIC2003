
#include "UsesProcessing.h"

void UsesProcessing::processUses(vector<StatementInfo>& statementInfo, multimap<int, int> parentChildMapping) {
    map<int, string> lineToTypeMapping;
    map<int, set<string>> usesMap;

    for (const auto& info : statementInfo) {
        lineToTypeMapping[info.lineCount] = info.statementType;
    }

    for (const auto& info : statementInfo) {
        vector<string> usedVariables;
        if (info.statementType == "assign") {
            usedVariables = extractAssignmentVariables(info.statementContent);
        } else if (info.statementType == "print") {
            usedVariables = extractPrintVariables(info.statementContent);
        }

        for (const auto& var : usedVariables) {
            updateUsesForAncestors(info.lineCount, var, parentChildMapping, lineToTypeMapping, usesMap);
        }
    }

    // Insert uses relationships into the database
    for (const auto& useEntry : usesMap) {
        for (const auto& variable : useEntry.second) {
            Database::insertUses(useEntry.first, variable);
        }
    }
}

void UsesProcessing::updateUsesForAncestors(int line, string variableName,  multimap<int, int> parentChildMapping, map<int, string> lineToTypeMapping, map<int, set<string>>& usesMap) {
    stack<int> ancestors;
    ancestors.push(line);

    while (!ancestors.empty()) {
        int currentLine = ancestors.top();
        ancestors.pop();

        usesMap[currentLine].insert(variableName);

        for (auto pair : parentChildMapping) {
            if (pair.second == currentLine) {
                ancestors.push(pair.first);
            }
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