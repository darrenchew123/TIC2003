#include "SyntaxValidator.h"


bool SyntaxValidator::checkName(const string& token){
    string namePattern = "[a-zA-Z][a-zA-Z0-9]*";
    return regex_match(token, regex(namePattern));
}
bool SyntaxValidator::isInteger(const string& intString) {
    if (intString.empty()) return false;
    int startIndex = 0;
    return all_of(intString.begin() + startIndex, intString.end(), ::isdigit);
}

bool SyntaxValidator::isVariable(const string& variable) {
    return !(variable.front() == '"' && variable.back() == '"');
}