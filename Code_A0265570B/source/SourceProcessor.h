#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Database.h"
#include "Tokenizer.h"

using namespace std;

class SourceProcessor {
private:
    void handleProcedure(const string& token, int& i, string& procedureName, bool& inProcedure);

    void handleStatement(const string& procedureName, const string& token, int& i, int lineCount, vector<string>& tokens);

    void handleAssignment(const string& procedureName, const string& varName, int& i, int lineCount, vector<string>& tokens);

public:
    void process(string& process);
};