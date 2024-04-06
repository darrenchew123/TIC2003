#pragma once

#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

#ifndef QUERY_H
#define QUERY_H

struct Condition {

    string conditionType;
    string leftArg;
    string rightArg;
    bool isT = false;

};

struct Pattern {

    string patternType;
    string patternVar;
    string patternLeftArg;
    string patternRightArg;
    bool isSubexpression = false;

};

struct Query {

    vector<Condition> conditions;
    vector<Pattern> patterns;
    // Using unordered_map for efficient search and retrieval
    //key = varName, value = varType
    unordered_map<string, string> declaredVariables;

    //multi selection types eg. Select <w1, w2, a1, a2> -> return while loops and assignments
    unordered_set<string> multiSelectType; // {while, if, assign}
    unordered_set<string> multiSelectVar; // {w1, w2, a1, a2}
    string selectType; // while
    string selectVar; // s1
};

#endif //QUERY_H
