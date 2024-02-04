#include "QueryProcessor.h"
#include "Tokenizer.h"

QueryProcessor::QueryProcessor() {}

QueryProcessor::~QueryProcessor() {}

bool QueryProcessor::typeValidator(const string &token){
    if(token == "procedure" || token == "variable" || token == "constant" || token == "assign" || token == "print" || token == "read" || token == "stmt")
        return true;
    return false;
}

void QueryProcessor::processObjects(const vector<string>& tokens, unordered_map<string, string> &declaredObjects) {
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

void QueryProcessor::processSelect(const vector<string>& tokens, unordered_map<string, string> declaredObjects, vector<pair<string, string>>& selectObjects) {
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

void QueryProcessor::parser(const vector<string>& tokens, vector<pair<string, string>>& selectObjects) {
    unordered_map<string, string> declaredObjects;

    processObjects(tokens,declaredObjects);
    processSelect(tokens,declaredObjects,selectObjects);
}

void QueryProcessor::evaluate(string query, vector<string>& output) {
	output.clear();

    vector<string> databaseResults;
    vector<pair<string, string>> selectObjects;
    Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(query, tokens);

    parser(tokens,selectObjects);


	for (int i = 0; i < selectObjects.size(); i++) {

		string selectType = selectObjects[i].first;

		if (selectType == "procedure") {
			Database::getProcedures(databaseResults);
		}
        else if (selectType == "variable") {
            Database::getVariables(databaseResults);
        }
//        else if (selectType == "constant") {
//            Database::getConstant(databaseResults);
//        }
//		else if (selectType == "assign") {
//			Database::getAssignments(databaseResults);
//		}
//		else if (selectType == "print") {
//			Database::getPrint(databaseResults);
//		}
//        else if (selectType == "read") {
//            Database::getRead(databaseResults);
//        }
        else if (selectType == "stmt") {
            Database::getStatements(databaseResults);
        }
	}
	for (string databaseResult : databaseResults) {
		output.push_back(databaseResult);
	}
}