#include "CallsProcessing.h"

void CallsProcessing::processCalls(vector<StatementInfo>& statementInfo) {
    for (const auto& info : statementInfo) {
        if (info.statementType == "call") {
            string calleeName = extractCalleeName(info.statementContent);
            if (!calleeName.empty()) {
                string callerName = info.procedureName;
                Database::insertCalls(callerName, calleeName);
            }
        }
    }
}

string CallsProcessing::extractCalleeName(const string& statement) {
    istringstream iss(statement);
    string calleeName;
    iss >> calleeName >> calleeName;
    if (!SyntaxValidator::checkName(calleeName)) {
        cerr << "Callee name does not adhere to naming convention: " << calleeName << endl;
        throw runtime_error("Callee name error");
    }

    return calleeName;}
