#include "CallsProcessing.h"

void CallsProcessing::processCalls(vector<StatementInfo>& statementInfo) {
    map<string, vector<string>> directCalls;
    for (const auto& info : statementInfo) {
        if (info.statementType == "call") {
            string calleeName = extractCalleeName(info.statementContent);
            if (!calleeName.empty()) {
                string callerName = info.procedureName;
                Database::insertCalls(callerName, calleeName);
                directCalls[callerName].push_back(calleeName);
            }
        }
    }
    insertTransientCalls(directCalls);
}

string CallsProcessing::extractCalleeName(const string& statement) {
    istringstream iss(statement);
    string calleeName;
    iss >> calleeName >> calleeName;
    if (!SyntaxValidator::checkName(calleeName)) {
        cerr << "Callee name does not adhere to naming convention: " << calleeName << endl;
        throw runtime_error("Callee name error");
    }

    return calleeName;
}

// Insert transient calls
void CallsProcessing::insertTransientCalls(const map<string, vector<string>>& directCalls) {
    map<string, set<string>> transientCalls;
    for (auto call : directCalls) {
        computeTransientForCaller(call.first, directCalls, transientCalls);
    }

    for (const auto& call : transientCalls) {
        for (const auto& callee : call.second) {
            Database::insertCallsT(call.first, callee);
        }
    }
}

void CallsProcessing::computeTransientForCaller(const string& caller, const map<string, vector<string>>& directCalls, map<string, set<string>>& transientCalls, set<string> visited) {
    if (visited.count(caller)) return;
    visited.insert(caller);

    if (directCalls.count(caller)) {
        for (auto callee : directCalls.at(caller)) {
            transientCalls[caller].insert(callee);
            computeTransientForCaller(callee, directCalls, transientCalls, visited);
            for (auto transitiveCallee : transientCalls[callee]) {
                transientCalls[caller].insert(transitiveCallee);
            }
        }
    }
}
