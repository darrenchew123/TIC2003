#include "QueryEvaluator.h"


QueryEvaluator::QueryEvaluator() {}

QueryEvaluator::~QueryEvaluator() {}


//Evalutate query to get result from DB
void QueryEvaluator::evaluate(string query, vector<string>& output) {
	output.clear();

    vector<string> databaseResults;
    vector<pair<string, string>> selectObjects;
    Tokenizer tk;
	vector<string> tokens;

	tk.tokenize(query, tokens);
    QueryParser::parser(tokens,selectObjects);


	for (int i = 0; i < selectObjects.size(); i++) {

		string selectType = selectObjects[i].first;

		if (selectType == "procedure") {
			Database::getProcedures(databaseResults);
		}
        else if (selectType == "variable") {
            Database::getVariables(databaseResults);
        }
        else if (selectType == "constant") {
            Database::getConstants(databaseResults);
        }
		else if (selectType == "assign") {
			Database::getStatementType(selectType,databaseResults);
		}
		else if (selectType == "print") {
			Database::getStatementType(selectType,databaseResults);
		}
        else if (selectType == "read") {
            Database::getStatementType(selectType,databaseResults);
        }
        else if (selectType == "stmt") {
            Database::getStatements(databaseResults);
        }
	}
	for (string databaseResult : databaseResults) {
		output.push_back(databaseResult);
	}
}