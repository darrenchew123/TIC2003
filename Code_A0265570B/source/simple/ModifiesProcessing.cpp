#include "ModifiesProcessing.h"

void ModifiesProcessing::processModifies(vector<StatementInfo>& statementInfo, multimap<int,int> parentChildMapping) {
    map<int, string> lineToTypeMapping;
    map<int, set<string>> modifiesMap;

    for (const auto& info : statementInfo) {
        lineToTypeMapping[info.lineCount] = info.statementType;
    }

    for (const auto& info : statementInfo) {
        std::string variableName = extractModifiesVariable(info.statementContent, info.statementType);
        if (!variableName.empty()) {
            updateModifiesForAncestors(info.lineCount, variableName, parentChildMapping, modifiesMap);
        }
    }
    for (const auto& modifyEntry : modifiesMap) {
        for (const auto& variable : modifyEntry.second) {
            Database::insertModifies(modifyEntry.first, variable);
        }
    }
}

void ModifiesProcessing::updateModifiesForAncestors(int line, string& variableName, multimap<int,int> parentChildMapping, map<int, set<string>>& modifiesMap) {
   stack<int> ancestors;
    ancestors.push(line);

    while (!ancestors.empty()) {
        int currentLine = ancestors.top();
        ancestors.pop();

        modifiesMap[currentLine].insert(variableName);
        for (const auto& pair : parentChildMapping) {
            if (pair.second == currentLine) {
                ancestors.push(pair.first);
            }
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