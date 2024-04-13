#include "Parent_HelperFunctions.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>

void Parent_HelperFunctions::handleWildcardLHS(string& getParentSQL, const string& selectVar, const string& selectType, const string& lhsSynType, const string& rhsSynType, const string& leftArg, const string& rightArg, bool isrhsSyn, vector<string>& results, vector<vector<string>>& dbResults) {

    
        cout << "lhs wildcard" << endl;

        if (isrhsSyn) { //rhs child syn
            cout << "rhs syn" << endl;
            if (selectType == "procedure") {
                getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine;";
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
                        + rightArg + "';";
                }
                else {
                    Database::getStatementType(selectType, results);
                    return;
                }
            }
            dbResults.clear();
        }
    
        
}

void Parent_HelperFunctions::handleSynonymLHS(string& getParentSQL, const string& selectVar, const string& selectType, const string& lhsSynType, const string& rhsSynType, const string& leftArg,const string& rightArg, bool isrhsSyn, vector<string>& results, vector<vector<string>>& dbResults) {
    
     //lhs is syn
        cout << "lhs is syn" << endl;
        if (rightArg == "_") {// rhs is wildcard
            cout << "rhs is wildcard" << endl;
            if (lhsSynType == "stmt") {
                if (selectType == "procedure") {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine;";
                }
                else {
                    getParentSQL = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation;";
                }
            }
            else {
                getParentSQL = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation INTERSECT SELECT codeLine FROM Statement WHERE statementType = '"
                    + selectType + "';";
            }
        }
        else if (isrhsSyn) { //rhs is syn
            cout << "rhs is syn" << endl;


            if (lhsSynType == "stmt" && rhsSynType == "stmt" && leftArg == selectVar) {
                cout << "Select s such that Parent(s, s1)" << endl;
                getParentSQL = "SELECT DISTINCT parentStatementCodeLine FROM ParentChildRelation;";
            }
            else if (lhsSynType == "stmt" && rhsSynType == "stmt" && rightArg == selectVar) {
                cout << "Select s1 such that Parent(s, s1)" << endl;
                getParentSQL = "SELECT DISTINCT childStatementCodeLine FROM ParentChildRelation;";
            }
            else if (lhsSynType == selectType) { //return parent
                cout << "return parent" << endl;
                if (lhsSynType == "stmt") {
                    cout << "Select s such that Parent(s, a)" << endl;
                    getParentSQL = "SELECT DISTINCT P.parentStatementCodeLine  FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S2.statementType = '"
                        + rhsSynType + "';";
                }

                else if (rhsSynType == "stmt") {
                    getParentSQL = "SELECT DISTINCT P.parentStatementCodeLine  FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "';";
                }
                else {
                    getParentSQL = "SELECT P.parentStatementCodeLine  FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "';";
                }
            }
            else if (rhsSynType == selectType) {//return child
                cout << "return child" << endl;
                if (rhsSynType == "stmt") {
                    getParentSQL = "SELECT DISTINCT P.childStatementCodeLine  FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "';";
                }
                else {
                    getParentSQL = "SELECT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                        + lhsSynType + "' AND S2.statementType = '"
                        + rhsSynType + "';";
                }
            }
            else if (selectType == "procedure") {
                cout << "select type procedure" << endl; {
                    if (lhsSynType == "stmt" && rhsSynType == "stmt") {
                        getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine;";
                    }
                    else if (rhsSynType == "stmt") {
                        getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.parentStatementCodeLine = S1.codeLine WHERE S1.statementType = '"
                            + lhsSynType + "';";
                    }
                    else if (lhsSynType == "stmt") {
                        getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine WHERE S1.statementType = '"
                            + rhsSynType + "';";
                    }
                    else {
                        getParentSQL = "SELECT DISTINCT S.procedureName FROM Statement S JOIN (SELECT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                            + lhsSynType + "' AND S2.statementType = '"
                            + rhsSynType + "') AS Subquery ON S.codeLine = Subquery.childStatementCodeLine;";
                    }
                }
            }
        }
        else {
            cout << "rhs stmt line" << endl;
            if (lhsSynType == selectType) {
                cout << "return parent" << endl;
                getParentSQL = "SELECT P.parentStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                    + lhsSynType + "' AND S2.codeLine = '"
                    + rightArg + "';";
            }
            else if (rhsSynType == selectType) {
                cout << "return child" << endl;
                getParentSQL = "SELECT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.statementType = '"
                    + lhsSynType + "' AND S2.codeLine = '"
                    + rightArg + "';";
            }
            else if (selectType == "procedure") {
                getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine WHERE S1.statementType = '"
                    + lhsSynType + "' UNION SELECT DISTINCT S2.procedureName FROM Statement S2 JOIN AncestorRelation A ON A.childStatementCodeLine = S2.codeLine WHERE S2.codeLine = '"
                    + rightArg + "';";
            }

        }
    
   
}

void Parent_HelperFunctions::handleLineNumberLHS(string& getParentSQL, const string& selectVar, const string& selectType, const string& lhsSynType, const string& rhsSynType, const string& leftArg, const string& rightArg, bool isrhsSyn, vector<string>& results, vector<vector<string>>& dbResults) {
    
    
        cout << "lhs stmt line" << endl;
        if (rightArg == "_") {
            if (selectType == "procedure") {
                getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.parentStatementCodeLine = S1.codeLine WHERE S1.codeLine = '"
                    + leftArg + "';";
            }
            else {
                Database::getStatementType(selectType, results);
                return;
            }
        }
        else if (isrhsSyn) {
            cout << "rhs syn" << endl;
            if (rhsSynType == "stmt") {
                if (selectType == "procedure") {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.parentStatementCodeLine = S1.codeLine WHERE S1.codeLine = '"
                        + leftArg + "';";
                }
                else if (rightArg == selectVar) {
                    getParentSQL = "SELECT childStatementCodeLine FROM ParentChildRelation WHERE parentStatementCodeLine = '"
                        + leftArg + "';";
                }
            }
            else if (rightArg == selectVar) {
                getParentSQL = "SELECT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.codeLine = '"
                    + leftArg + "' AND S2.statementType = '"
                    + rhsSynType + "';";
            }
            else {
                if (selectType == "variable") {
                    cout << "return respective var" << endl;
                    getParentSQL = "SELECT DISTINCT V.variableName FROM Variable V JOIN (SELECT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.codeLine = '"
                        + leftArg + "' AND S2.statementType = '"
                        + rhsSynType + "') AS Subquery ON V.statementCodeLine = Subquery.childStatementCodeLine;";
                }
                else if (selectType == "constant") {
                    cout << "return respective constantValue" << endl;
                    getParentSQL = "SELECT DISTINCT C.constantValue FROM Constant C JOIN (SELECT P.childStatementCodeLine FROM ParentChildRelation P JOIN Statement S1 ON P.parentStatementCodeLine = S1.codeLine JOIN Statement S2 ON P.childStatementCodeLine = S2.codeLine WHERE S1.codeLine = '"
                        + leftArg + "' AND S2.statementType = '"
                        + rhsSynType + "') AS Subquery ON C.statementCodeLine = Subquery.childStatementCodeLine;";
                }
                else if (selectType == "procedure") {
                    getParentSQL = "SELECT DISTINCT S1.procedureName FROM Statement S1 JOIN ParentChildRelation P ON P.childStatementCodeLine = S1.codeLine WHERE S1.statementType = '"
                        + rhsSynType + "';";
                }
            }
        }
        else {
            cout << "rhs lineNumber" << endl;

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
                        + leftArg + "';";
                }
                else {
                    getParentSQL = "SELECT DISTINCT CodeLine FROM Statement WHERE statementType ='"
                        + selectType + "';";
                }
            }
            dbResults.clear();
        }
    

}



