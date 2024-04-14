#include "HandleMultipleSelect.h"

void HandleMultipleSelect::processMultiSelectQuery(string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute, bool isMultipleCond){
    vector<vector<string>> allResults;
    vector<string> combinedResults;
    unordered_map<string, pair<int, vector<string>>> resultsMap;
    cout << "Multi Select Variables:\n";
    for (auto var : queryToExecute.multiSelectVar) {
        cout << "  Var: " << var << endl;
    }
    cout << "Declared Variables:\n";
    for (auto var : queryToExecute.declaredVariables) {
        cout << "  Var: " << var.first << ", Type: " << var.second << endl;
    }

    int order = 0;
    for ( auto selectVar : queryToExecute.multiSelectVar) {
        vector<string> tempResults;
        if(isMultipleCond){
            HandleMultipleConditions::processSingleSelectMultiCond(selectVar, queryToExecute.declaredVariables[selectVar], conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, tempResults, queryToExecute);
        } else {
            HandleSimpleQueries::processSimpleQuery(selectVar, queryToExecute.declaredVariables[selectVar], conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, tempResults, queryToExecute);
        }
        cout << "Processing query for variable: " << selectVar << endl;
        for (auto result : tempResults) {
            cout << "  Result: " << result << endl;
        }
        allResults.push_back(tempResults);
        resultsMap[selectVar] = make_pair(order++, tempResults);
    }
    if (allResults.empty()) return;
    cout <<queryToExecute.conditions.empty() << endl;
    
    if(!queryToExecute.conditions.empty()) postProcessConditions(resultsMap,combinedResults,queryToExecute);
    else postProcessNoConditions(allResults,combinedResults);

    databaseResults = combinedResults;
}

void HandleMultipleSelect::postProcessNoConditions(vector<vector<string>>  & allResults, vector<string> &combinedResults){
    vector<vector<string>> product;

    for (auto item : allResults.front()) {
        product.push_back({item});
    }
    for (int i = 1; i < allResults.size(); ++i) {
        vector<vector<string>> tempProduct;

        for (auto acc : product) {
            for (auto item : allResults[i]) {
                auto accCopy = acc;
                accCopy.push_back(item);
                tempProduct.push_back(accCopy);
            }
        }

        product = tempProduct;
    }
    for (auto items : product) {
        string concatenatedResult;
        for (auto item : items) {
            if (!concatenatedResult.empty()) concatenatedResult += " ";
            concatenatedResult += item;
        }
        combinedResults.push_back(concatenatedResult);
    }
};


void HandleMultipleSelect::postProcessConditions(unordered_map<string, pair<int, vector<string>>> resultsMap, vector<string>& combinedResults, const Query& queryToExecute) {
    if (resultsMap.empty()) {
        cout << "No entities specified in the results map." << endl;
        return;
    }

    vector<pair<int, string>> orderedEntities;
    for (const auto& entry : resultsMap) {
        orderedEntities.push_back(make_pair(entry.second.first, entry.first));
    }
    sort(orderedEntities.begin(), orderedEntities.end());

    vector<string> entityOrder;
    for (const auto& entry : orderedEntities) {
        entityOrder.push_back(entry.second);
    }

    vector<vector<string>> allCombinations;
    vector<string> current;

    generateCombinations(resultsMap, entityOrder, 0, current, allCombinations);

    vector<vector<string>> validCombinations = allCombinations;  // Start with all combinations as valid

    for (const auto& condition : queryToExecute.conditions) {
        vector<vector<string>> tempValidCombinations;

        for (const auto& combo : validCombinations) {
            auto leftIndex = find(entityOrder.begin(), entityOrder.end(), condition.leftArg) - entityOrder.begin();
            auto rightIndex = find(entityOrder.begin(), entityOrder.end(), condition.rightArg) - entityOrder.begin();

            if (checkRelationship(condition.conditionType, combo[leftIndex], combo[rightIndex], condition.isT,queryToExecute,condition)) {
                tempValidCombinations.push_back(combo);
            }
        }

        validCombinations = tempValidCombinations;
    }
    for (const auto& combo : validCombinations) {
        stringstream result;
        copy(combo.begin(), combo.end(), ostream_iterator<string>(result, " "));
        string finalResult = result.str();
        finalResult.pop_back();  // Remove the last space
        combinedResults.push_back(finalResult);
    }
}


bool HandleMultipleSelect::checkRelationship(string relationshipType, string entity1, string entity2, bool isT, Query queryToExecute, Condition condition) {
    if (relationshipType == "Calls" && isT) {
        return Database::checkCallsTRelationship(entity1, entity2);
    }
    else if (relationshipType == "Parent" && isT) {
        return Database::checkParentTRelationship(entity1, entity2);
    }
    else if (relationshipType == "Calls") {
        return Database::checkCallsRelationship(entity1, entity2);
    }
    else if (relationshipType == "Parent") {
        return Database::checkParentRelationship(entity1, entity2);
    }
    else if (relationshipType == "Modifies") {
        return Database::checkModifiesRelationship(entity1, entity2, queryToExecute.declaredVariables[condition.leftArg]);
    }
    return false;
}

// Recursive function to generate all combinations of entities
void HandleMultipleSelect::generateCombinations(unordered_map<string, pair<int, vector<string>>> resultsMap, vector<string>& entityOrder, int index, vector<string>& current, vector<vector<string>>& allCombinations) {
    if (index == entityOrder.size()) {
        allCombinations.push_back(current);
        return;
    }

    string key = entityOrder[index];
    const vector<string>& entities = resultsMap.at(key).second;
    for (string entity : entities) {
        current.push_back(entity);
        generateCombinations(resultsMap, entityOrder, index + 1, current, allCombinations);
        current.pop_back();
    }
}