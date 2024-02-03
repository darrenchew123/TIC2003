#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Database.h"
#include "Tokenizer.h"

using namespace std;

class SourceProcessor {
private:
    void incrementLineCount(int& lineCount);

    void handleProcedure(const string& token, size_t& i, string& procedureName, bool& inProcedure);

    void handleStatement(const string& procedureName, const string& token, size_t& i, int lineCount, vector<string>& tokens);

    void handleAssignment(const string& procedureName, const string& varName, size_t& i, int lineCount, vector<string>& tokens);

public:
    void process(string program);
};