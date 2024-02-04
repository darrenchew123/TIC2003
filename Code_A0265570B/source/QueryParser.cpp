#include "QueryParser.h"


//Validate token type
bool QueryParser::typeValidator(const string &token){
    if(token == "procedure" || token == "variable" || token == "constant" || token == "assign" || token == "print" || token == "read" || token == "stmt")
        return true;
    return false;
}

//Process query file to store declared objects that will be used fir validation by processSelect
void QueryParser::processObjects(const vector<string>& tokens, unordered_map<string, string> &declaredObjects) {
    //Process declared object eg. variable v;,constant c;
    for (int i = 0; i < tokens.size(); i++) {
        if (typeValidator(tokens[i])) {
            if (i + 1 < tokens.size()) {
                declaredObjects[tokens[i + 1]] = tokens[i];
                i += 1;
            }
        }
    }
}

//Process query file to store into selectObjects to be processed by statements
void QueryParser::processSelect(const vector<string>& tokens, unordered_map<string, string> declaredObjects, vector<pair<string, string>>& selectObjects) {
    for (int i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "Select") {
            if (i + 1 < tokens.size()) {
                string selectIdentifier = tokens[i + 1];
                if (declaredObjects.find(selectIdentifier) != declaredObjects.end()) {
                    selectObjects.push_back(make_pair(declaredObjects[selectIdentifier], selectIdentifier));
                } else {
                    throw runtime_error("Error: Identifier '" + selectIdentifier + "' not declared.");
                }
            }
        }
    }
}

//Start process to parse query
void QueryParser::parser(const vector<string>& tokens, vector<pair<string, string>>& selectObjects) {
    unordered_map<string, string> declaredObjects;
    processObjects(tokens,declaredObjects);
    processSelect(tokens,declaredObjects,selectObjects);
}
