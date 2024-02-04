#pragma once

#include <string>
#include <vector>
#include "Database.h"
#include "Tokenizer.h"
#include "QueryParser.h"



using namespace std;

class QueryEvaluator {

public:
	QueryEvaluator();

    ~QueryEvaluator();

	void evaluate(string query, vector<string>& results);
};