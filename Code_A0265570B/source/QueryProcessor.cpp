#include "QueryProcessor.h"
#include "Tokenizer.h"

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}

// method to evaluate a query
// This method currently only handles queries for getting all the procedure names,
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all required queries.
void QueryProcessor::evaluate(string query, vector<string>& output) {
	// clear the output vector
	output.clear();

	// tokenize the query
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(query, tokens);

	// create a vector for storing the results from database
	vector<string> databaseResults;

	//Process each token, check its type and get from its respective database
	for (int i = 0; i < tokens.size(); i++) {

		string synonymType = tokens[i];

		//check for a procedure
		if (synonymType == "procedure") {

			Database::getProcedures(databaseResults);
		}

		//check for a read statement
		else if (synonymType == "read") {

			//Database::getProcedures(databaseResults);
		}

		//check for a call statement
		else if (synonymType == "call") {

			//Database::getProcedures(databaseResults);
		}

		//check for a print statement
		else if (synonymType == "print") {

			//Database::getProcedures(databaseResults);
		}

		//check for a constant
		else if (synonymType == "constant") {

			//Database::getProcedures(databaseResults);
		}
		
		//check for variable

		//check for assigment

		//check for statement

	}

	// post process the results to fill in the output vector
	for (string databaseResult : databaseResults) {
		output.push_back(databaseResult);
	}
}