#include "HandleMultipleSelect.h"

void HandleMultipleSelect::processMultiSelectQuery(string conditionType, bool isT, string leftArg, string rightArg, string patternType, string patternLeftArg, string patternRightArg, bool isSubexpression, vector<string>& databaseResults, Query queryToExecute, bool isMultipleCond){
    vector<vector<string>> allResults;
    vector<string> combinedResults;
    std::cout << "Multi Select Variables:\n";
    for (const auto& var : queryToExecute.multiSelectVar) {
        std::cout << "  Var: " << var << std::endl;
    }
    std::cout << "Declared Variables:\n";
    for (const auto& var : queryToExecute.declaredVariables) {
        std::cout << "  Var: " << var.first << ", Type: " << var.second << std::endl;
    }

    for ( auto selectVar : queryToExecute.multiSelectVar) {
        vector<string> tempResults;
        if(isMultipleCond){
            HandleMultipleConditions::processSingleSelectMultiCond(selectVar, queryToExecute.declaredVariables[selectVar], conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, tempResults, queryToExecute);
        } else {
            HandleSimpleQueries::processSimpleQuery(selectVar, queryToExecute.declaredVariables[selectVar], conditionType, isT, leftArg, rightArg, patternType, patternLeftArg, patternRightArg, isSubexpression, tempResults, queryToExecute);
        }
        std::cout << "Processing query for variable: " << selectVar << std::endl;
        for (const auto& result : tempResults) {
            std::cout << "  Result: " << result << std::endl;
        }
        allResults.push_back(tempResults);
    }
    if (allResults.empty()) return;
    cout <<queryToExecute.conditions.empty() << endl;
    // HERE GOT BUG NEED FIX THE CALLS INPUT AS A CONDITIONS IN PARSER
    if(queryToExecute.conditions.empty()) postProcessConditions(allResults,combinedResults,queryToExecute);
    else postProcessNoConditions(allResults,combinedResults);

    databaseResults = combinedResults;
}

void HandleMultipleSelect::postProcessNoConditions(vector<vector<string>>  & allResults, vector<string> &combinedResults){
    std::vector<std::vector<std::string>> product;

    for (const auto& item : allResults.front()) {
        product.push_back({item});
    }
    for (int i = 1; i < allResults.size(); ++i) {
        std::vector<std::vector<std::string>> tempProduct;

        for (const auto& acc : product) {
            for (const auto& item : allResults[i]) {
                auto accCopy = acc;
                accCopy.push_back(item);
                tempProduct.push_back(accCopy);
            }
        }

        product = tempProduct;
    }
    for (auto items : product) {
        std::string concatenatedResult;
        for (const auto& item : items) {
            if (!concatenatedResult.empty()) concatenatedResult += " ";
            concatenatedResult += item;
        }
        combinedResults.push_back(concatenatedResult);
    }
};

void HandleMultipleSelect::postProcessConditions(vector<vector<string>>  & allResults, vector<string> &combinedResults, Query queryToExecute){
    for (const auto& entity1 : allResults[0]) {
        for (const auto& entity2 : allResults[1]) {
            std::cout << "Checking relationships between " << entity1 << " and " << entity2 << std::endl;

            bool valid = true;
            if (!HandleMultipleSelect::checkRelationship("calls", entity1, entity2, queryToExecute)) {
                valid = false;
            }
            else {
                combinedResults.push_back(entity1 + " " + entity2);
            }

        }
    }
}

bool HandleMultipleSelect::checkRelationship(const std::string& relationshipType, const std::string& entity1, const std::string& entity2, const Query& query) {
    if (relationshipType == "calls") {
        return Database::checkCallsRelationship(entity1, entity2);
    }
    return false;
}