#include "ParentT_HelperFunctions.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>

void ParentT_HelperFunctions::handleWildcardLHS(string& getParentSQL, const string& selectVar, const string& selectType, const string& lhsSynType, const string& rhsSynType, const string& leftArg, const string& rightArg, bool isrhsSyn, bool AncestorExists, vector<string>& results, vector<vector<string>>& dbResults) {

        cout << "lhs wildcard" << endl;
        if (isrhsSyn) { //rhs child syn
            if (selectType == "procedure") {
                getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine UNION SELECT DISTINCT S2.procedureName FROM Statement S2 JOIN AncestorRelation A ON A.childStatementCodeLine = S2.codeLine;";
            }
            else {
                getParentSQL = "SELECT Statement.codeLine FROM(SELECT childStatementCodeLine FROM ParentChildRelation UNION SELECT childStatementCodeLine FROM AncestorRelation) AS A JOIN Statement ON A.childStatementCodeLine = Statement.codeLine WHERE Statement.statementType ='"
                    + selectType + "'; ";
            }
        }
        else if (rightArg == "_") {
            cout << "rhs wildcard" << endl;
            if (selectType == "constant") {
                getParentSQL = "SELECT DISTINCT constantValue FROM Constant";
            }
            else if (selectType == "variable") {
                getParentSQL = "SELECT DISTINCT variableName FROM Variable";
            }
            else if (selectType == "stmt") {
                getParentSQL = "SELECT DISTINCT CodeLine FROM Statement";
            }
            else if (selectType == "procedure") {
                getParentSQL = "SELECT * FROM Procedure";
            }
            else {
                Database::getStatementType(selectType, results);
                return;
            }
        }
        else { //rhs stmtline

            if (!dbResults.empty()) {
                cout << "true" << endl;
                if (selectType == "constant") {
                    getParentSQL = "SELECT DISTINCT constantValue FROM Constant";
                }
                else if (selectType == "variable") {
                    getParentSQL = "SELECT DISTINCT variableName FROM Variable";
                }
                else if (selectType == "stmt") {
                    getParentSQL = "SELECT DISTINCT CodeLine FROM Statement";
                }
                else if (selectType == "procedure") {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine WHERE S1.codeLine = '"
                        + rightArg + "' UNION SELECT DISTINCT S2.procedureName FROM Statement S2 JOIN AncestorRelation A ON A.childStatementCodeLine = S2.codeLine WHERE S2.codeLine = '"
                        + rightArg + "';";
                }
                else {
                    Database::getStatementType(selectType, results);
                    return;
                }
            }
            dbResults.clear();
        }
    
    return;
}

void ParentT_HelperFunctions::handleSynonymLHS(string& getParentSQL, const string& selectVar, const string& selectType, const string& lhsSynType, const string& rhsSynType, const string& leftArg,const string& rightArg, bool isrhsSyn, bool AncestorExists, vector<string>& results, vector<vector<string>>& dbResults) {
    
    
    //lhs is syn
        cout << "lhs is syn" << endl;
        if (lhsSynType == "stmt") {
            cout << "lhs syn is stmt" << endl;
            if (isrhsSyn) {
                cout << "rhs is syn" << endl;
                if (rhsSynType == "stmt") {
                    cout << "rhs is syn stmt (s, s1)" << endl;
                    if (selectType == "procedure" && AncestorExists) {
                        getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine UNION SELECT DISTINCT S2.procedureName FROM Statement S2 JOIN AncestorRelation A ON A.childStatementCodeLine = S2.codeLine;";
                    }
                    else if (selectType == "procedure" && !AncestorExists) {
                        getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine";
                    }
                    else if (leftArg == selectVar) {
                        cout << "return parent" << endl;
                        getParentSQL = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation";
                    }
                    else if (rightArg == selectVar) {
                        cout << "return child" << endl;
                        getParentSQL = "SELECT DISTINCT childStatementCodeLine FROM ParentChildRelation";
                    }
                }
                else if (AncestorExists) { // to encapsulate
                    cout << "got ancestor" << endl;
                    if (leftArg == selectVar) { //return parent
                        cout << "return parent (s, w)" << endl;
                        getParentSQL = "SELECT P.parentStatementCodeLine AS ParentLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S2.statementType = '"
                            + rhsSynType + "' UNION SELECT A.ancestorStatementCodeLine AS ParentLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S3.statementType = '"
                            + rhsSynType + "';";
                    }
                    else if (rightArg == selectVar) {
                        cout << "return child (s,w)" << endl;
                        getParentSQL = "SELECT P.childStatementCodeLine AS ParentLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S2.statementType = '"
                            + rhsSynType + "' UNION SELECT A.childStatementCodeLine AS ParentLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S3.statementType = '"
                            + rhsSynType + "';";
                    }
                }
            }
            else if (rightArg == "_") {
                cout << "rhs wildcard (s, _)" << endl;
                if (selectType == "procedure" && AncestorExists) {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine UNION SELECT DISTINCT S2.procedureName FROM Statement S2 JOIN AncestorRelation A ON A.childStatementCodeLine = S2.codeLine;";
                }
                else if (selectType == "procedure" && !AncestorExists) {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine";
                }
                else
                {
                    getParentSQL = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation";
                }
            }
            else {
                cout << "rhs is line stmt ie (s, 9)" << endl;
                if (selectType == "procedure" && AncestorExists) {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine WHERE S1.codeLine = '"
                        + rightArg + "' UNION SELECT DISTINCT S2.procedureName FROM Statement S2 JOIN AncestorRelation A ON A.childStatementCodeLine = S2.codeLine WHERE S2.codeLine = '"
                        + rightArg + "';";
                }
                else if (selectType == "procedure" && !AncestorExists) {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine WHERE S1.codeLine = '"
                        + rightArg + "';";
                }
                else {
                    getParentSQL = "SELECT P.parentStatementCodeLine AS ParentLine FROM ParentChildRelation P WHERE P.childStatementCodeLine = '"
                        + rightArg + "' UNION SELECT A.ancestorStatementCodeLine AS ParentLine FROM AncestorRelation A WHERE A.childStatementCodeLine = '"
                        + rightArg + "';";
                }
            }
        }
        else if (rightArg == "_") {// rhs is wildcard
            cout << "rhs is wildcard" << endl;
            if (selectType == "procedure" && AncestorExists) {
                getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.parentStatementCodeLine = S1.codeLine WHERE S1.statementType = '"
                    + lhsSynType + "' UNION SELECT DISTINCT S2.procedureName FROM Statement S2 JOIN AncestorRelation A ON A.ancestorStatementCodeLine = S2.codeLine WHERE S2.statementType = '"
                    + lhsSynType + "';";
            }
            else if (selectType == "procedure" && !AncestorExists) {
                getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.parentStatementCodeLine = S1.codeLine WHERE S1.statementType = '"
                    + lhsSynType + "';";
            }
            else if (lhsSynType == "stmt") {
                getParentSQL = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation;";
            }
            else {
                getParentSQL = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation INTERSECT SELECT codeLine FROM Statement WHERE statementType = '"
                    + selectType + "';";
            }
        }
        else if (isrhsSyn) { //rhs is syn
            cout << "rhs is syn" << endl;

            if (rhsSynType == "stmt") {
                cout << "Parent*(w, s)" << endl;
                if (leftArg == selectVar) {
                    cout << "return parent" << endl;
                    getParentSQL = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation INTERSECT SELECT codeLine FROM Statement WHERE statementType = '"
                        + selectType + "';";
                }
                else if (rightArg == selectVar) {
                    cout << "return child" << endl;
                    getParentSQL = "SELECT P.childStatementCodeLine AS ParentLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' UNION SELECT A.childStatementCodeLine AS ParentLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "';";
                }
            }

            else if (AncestorExists) { // to encapsulate
                cout << "got ancestor" << endl;
                cout << leftArg << " " << selectVar << endl;
                if (leftArg == selectVar) { //return parent
                    cout << "return parent" << endl;
                    getParentSQL = "SELECT P.parentStatementCodeLine AS ParentLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "' UNION SELECT A.ancestorStatementCodeLine AS ParentLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S3.statementType = '"
                        + rhsSynType + "';";
                }
                else if (rightArg == selectVar) {//return child syn, solves multiselect case
                    cout << "return child" << endl;
                    getParentSQL = "SELECT P.childStatementCodeLine AS ChildLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "' UNION SELECT A.childStatementCodeLine AS ChildLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S3.statementType = '"
                        + rhsSynType + "';";
                }
                else if (selectType == "variable") {
                    cout << "return respective var" << endl;
                    getParentSQL = "SELECT DISTINCT V.variableName FROM Variable V JOIN (SELECT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "' UNION SELECT A.childStatementCodeLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S3.statementType = '"
                        + rhsSynType + "') AS Subquery ON V.statementCodeLine = Subquery.childStatementCodeLine;";
                }
                else if (selectType == "constant") {
                    cout << "return respective constantValue" << endl;
                    getParentSQL = "SELECT DISTINCT C.constantValue FROM Constant C JOIN (SELECT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "' UNION SELECT A.childStatementCodeLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S3.statementType = '"
                        + rhsSynType + "') AS Subquery ON C.statementCodeLine = Subquery.childStatementCodeLine;";
                }
                else if (selectType == "procedure") {
                    cout << "return respective procedure" << endl;
                    getParentSQL = "SELECT DISTINCT S.procedureName FROM Statement S JOIN (SELECT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "' UNION SELECT A.childStatementCodeLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S3.statementType = '"
                        + rhsSynType + "') AS Subquery ON S.codeLine = Subquery.childStatementCodeLine;";
                }
                else if (lhsSynType == selectType) { //return parent
                    cout << "return parent" << endl;
                    getParentSQL = "SELECT P.parentStatementCodeLine AS ParentLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "' UNION SELECT A.ancestorStatementCodeLine AS ParentLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S3.statementType = '"
                        + rhsSynType + "';";
                }
                else if (rhsSynType == selectType) {// return
                    cout << "return child; matching select type" << endl;
                    getParentSQL = "SELECT P.childStatementCodeLine AS ChildLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "' UNION SELECT A.childStatementCodeLine AS ChildLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S3.statementType = '"
                        + rhsSynType + "';";
                }
            }
            else {
                cout << "no ancestor" << endl;
                if (leftArg == selectVar) { //return parent
                    cout << "return parent including multi" << endl;
                    getParentSQL = "SELECT DISTINCT P.parentStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "';";
                }
                else if (rightArg == selectVar) {//return child syn
                    cout << "return child" << endl;
                    getParentSQL = "SELECT DISTINCT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "';";

                }
                else if (selectType == "variable") {
                    cout << "return respective var" << endl;
                    getParentSQL = "SELECT DISTINCT V.variableName FROM Variable V JOIN (SELECT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "') AS Subquery ON V.statementCodeLine = Subquery.childStatementCodeLine;";
                }
                else if (selectType == "procedure") {
                    cout << "return respective procedure" << endl;
                    getParentSQL = "SELECT DISTINCT S.procedureName FROM Statement S JOIN (SELECT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "') AS Subquery ON S.codeLine = Subquery.childStatementCodeLine;";
                }
                else if (lhsSynType == selectType) {
                    cout << "return parent" << endl;
                    getParentSQL = "SELECT DISTINCT P.parentStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "';";
                }
                else if (rhsSynType == selectType) {
                    cout << "return child" << endl;
                    getParentSQL = "SELECT DISTINCT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "';";
                }
            }
        }
        else {
            cout << "rhs is linenumber" << endl;
            getParentSQL = "SELECT P.parentStatementCodeLine AS ParentLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                + lhsSynType + "' AND S2.codeLine = '"
                + rightArg + "' UNION SELECT A.ancestorStatementCodeLine AS ParentLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S1.statementType = '"
                + lhsSynType + "' AND S3.codeLine = '"
                + rightArg + "';";
        }
    
    return;
}

void ParentT_HelperFunctions::handleLineNumberLHS(string& getParentSQL, const string& selectVar, const string& selectType, const string& lhsSynType, const string& rhsSynType, const string& leftArg, const string& rightArg, bool isrhsSyn, bool AncestorExists, vector<string>& results, vector<vector<string>>& dbResults) {
    
    
        cout << "lhs linenumber line" << endl;
        if (rightArg == "_") {
            if (selectType == "procedure" && AncestorExists) {
                getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.parentStatementCodeLine = S1.codeLine WHERE S1.codeLine = '"
                    + leftArg + "' UNION SELECT DISTINCT S2.procedureName FROM Statement S2 JOIN AncestorRelation A ON A.ancestorStatementCodeLine = S2.codeLine WHERE S2.codeLine = '"
                    + leftArg + "';";
            }
            else if (selectType == "procedure" && !AncestorExists) {
                getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.parentStatementCodeLine = S1.codeLine WHERE S1.codeLine = '"
                    + leftArg + "';";
            }
            else {
                Database::getStatementType(selectType, results);
                return;
            }
        }
        else if (isrhsSyn) {
            cout << "rhs syn" << endl; //to factor stmt syn

            if (rhsSynType == "stmt") {
                cout << "rhs syn is stmt" << endl;
                if (selectType == "procedure" && AncestorExists) {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.parentStatementCodeLine = S1.codeLine WHERE S1.codeLine = '"
                        + leftArg + "' UNION SELECT DISTINCT S2.procedureName FROM Statement S2 JOIN AncestorRelation A ON A.ancestorStatementCodeLine = S2.codeLine WHERE S2.codeLine = '"
                        + leftArg + "';";
                }
                else if (selectType == "procedure" && !AncestorExists) {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.parentStatementCodeLine = S1.codeLine WHERE S1.codeLine = '"
                        + leftArg + "';";
                }
                else if (rightArg == selectVar) {
                    getParentSQL = "SELECT P.childStatementCodeLine AS ParentLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.codeLine = '"
                        + leftArg + "' UNION SELECT A.childStatementCodeLine AS ParentLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S1.codeLine = '"
                        + leftArg + "';";
                }
            }


            else if (rightArg == selectVar) {
                cout << "return child" << endl;
                getParentSQL = "SELECT P.childStatementCodeLine AS ChildCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.codeLine = '"
                    + leftArg + "' AND S2.statementType = '"
                    + rhsSynType + "' UNION SELECT A.childStatementCodeLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S1.codeLine = '"
                    + leftArg + "' AND S3.statementType = '"
                    + rhsSynType + "';";
            }
            else if (rhsSynType == selectType) {
                cout << "return child" << endl;
                getParentSQL = "SELECT P.childStatementCodeLine AS ChildCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.codeLine = '"
                    + leftArg + "' AND S2.statementType = '"
                    + rhsSynType + "' UNION SELECT A.childStatementCodeLine FROM AncestorRelation A JOIN Statement S1 ON A.ancestorStatementCodeLine = S1.codeLine JOIN Statement S3 ON A.childStatementCodeLine = S3.codeLine WHERE S1.codeLine = '"
                    + leftArg + "' AND S3.statementType = '"
                    + rhsSynType + "';";
            }
            else {
                if (selectType == "procedure" && AncestorExists) {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine WHERE S1.statementType = '"
                        + rhsSynType + "' UNION SELECT DISTINCT S2.procedureName FROM Statement S2 JOIN AncestorRelation A ON A.childStatementCodeLine = S2.codeLine WHERE S2.statementType = '"
                        + rhsSynType + "';";
                }
                else if (selectType == "procedure" && !AncestorExists) {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine WHERE S1.statementType = '"
                        + rhsSynType + "';";
                }
            }
        }
        else {
            cout << "rhs linenumber" << endl;

            if (!dbResults.empty()) {
                cout << "true" << endl;
                if (selectType == "constant") {
                    getParentSQL = "SELECT DISTINCT constantValue FROM Constant";
                }
                else if (selectType == "variable") {
                    getParentSQL = "SELECT DISTINCT variableName FROM Variable";
                }
                else if (selectType == "stmt") {
                    getParentSQL = "SELECT DISTINCT CodeLine FROM Statement";
                }
                else if (selectType == "procedure") {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.parentStatementCodeLine = S1.codeLine WHERE S1.codeLine = '"
                        + leftArg + "' UNION SELECT DISTINCT S2.procedureName FROM Statement S2 JOIN AncestorRelation A ON A.ancestorStatementCodeLine = S2.codeLine WHERE S2.codeLine = '"
                        + leftArg + "';";
                }
            }
            dbResults.clear();
        }
    

    return;
}



