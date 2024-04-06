#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include<iostream>
#include<stack>
#include "../db/Database.h"
#include "../utils/Tokenizer.h"
#include "ProcedureProcessing.h"
#include "ModifiesProcessing.h"
#include "UsesProcessing.h"
#include "CallsProcessing.h"
#include  "StatementInfo.h"
#include "AncestorProcessing.h"


using namespace std;

class SourceProcessor {
public:
    void process(string& process);

};