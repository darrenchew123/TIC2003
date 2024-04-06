
#include "StatementProcessing.h"

// Process statement and insert into statement table
void StatementProcessing::processStatement(const string& procedureName, const string& token, int& i, int& lineCount, const vector<string>& tokens, stack<string>& statementTypes, stack<int>& parentStack, vector<StatementInfo> &statementInfo, bool& pendingParentPush, multimap<int,int> &parentChildMapping) {
    if(statementTypes.size() == 0) return;
    cout << "Processing statement type :" << statementTypes.top() << " at line count: " << lineCount;
    if(!parentStack.empty()){
        cout << " Parent line code is :" << parentStack.top() << endl;
    }
    string statementContent;
    int counter = i - 2;
    while(counter>=0 && tokens[counter]!= "\n"){
        statementContent = tokens[counter] + " " + statementContent;
        counter--;
    }
    statementInfo.push_back({lineCount, statementContent, statementTypes.top(), procedureName});
    Database::insertStatement(procedureName, statementTypes.top(), statementContent, lineCount);
    statementTypes.pop();
    if (!parentStack.empty() && parentStack.top()!=lineCount) {
        cout << "Inserting parentChild parent :" << parentStack.top() << " child: " << lineCount << endl;
        int parentLine = parentStack.top();
        parentChildMapping.insert({parentLine,lineCount});
        Database::insertParentChildRelation(parentLine, lineCount);
    }
    if (pendingParentPush ) {
        cout << "Pushing into parent stack: " << lineCount << endl;
        parentStack.push(lineCount );
        pendingParentPush = false;
    }

}

//Process read print assignment logic and add into statement type
void StatementProcessing::processStatementStack(const string& token, stack<string>& statementTypes) {
    if(token == "=") statementTypes.push("assign");
    else statementTypes.push(token);
    cout << "Pushed to statementTypes: " << token << endl;
}