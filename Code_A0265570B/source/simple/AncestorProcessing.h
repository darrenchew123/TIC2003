#pragma once
#include <sstream>
#include <vector>
#include<iostream>
#include<queue>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include "../db/Database.h"
#include  "StatementInfo.h"


class AncestorProcessing {
public:
    static void processAncestor(multimap<int,int> parentChildMapping);

};


