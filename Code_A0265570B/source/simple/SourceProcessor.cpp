#include "SourceProcessor.h"


//Parse source program
void SourceProcessor::process(string &program) {
    // Initialize the database
    Database::initialize();

    // Tokenize the program
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(program, tokens);
    stack<string> statementTypes;
    stack<bool> expressionStack;
    stack<int> parentStack, ifStack;
    string lhs, rhs;
    vector<StatementInfo> statementInfo;
    multimap<int,int> parentChildMapping;
    bool pendingParentPush = false;


    int lineCount = 1;
    int blockDepth = 0;
    bool inProcedure = false;
    string procedureName;

    for (int i = 0; i < tokens.size(); i++) {
        string token = tokens[i];
        cout << "Processing token: " << token << " at index: " << i << endl;

        if (token == "procedure") {
            blockDepth = 0;
            ProcedureProcessing::processProcedure(inProcedure, procedureName, i, tokens);
            inProcedure = true;
        }

        if (inProcedure) {
            if (token != "procedure") {
                ProcedureProcessing::processInProcedure(token, procedureName, i, lineCount, tokens, statementTypes, parentStack, expressionStack, statementInfo, ifStack, pendingParentPush, parentChildMapping);
            }
            if (token == "{") {
                blockDepth++;
            } else if (token == "}") {
                if (blockDepth > 0) {
                    blockDepth--;
                } else {
                    inProcedure = false;
                }
            }
        }
    }

    ExpressionProcessing::processExpression(statementInfo);
    ModifiesProcessing::processModifies(statementInfo,parentChildMapping);
    UsesProcessing::processUses(statementInfo);
    CallsProcessing::processCalls(statementInfo);
    AncestorProcessing::processAncestor(parentChildMapping);
}