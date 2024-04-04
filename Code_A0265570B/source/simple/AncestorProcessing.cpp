#include "AncestorProcessing.h"
#include "../db/Database.h"

using namespace std;

void AncestorProcessing::processAncestor(unordered_map<int,int> parentChildMapping) {

    map<int, int> childParentPair;

    for (auto p : parentChildMapping) {
        childParentPair[p.second] = p.first;
    }

    multimap<int, int> grandchildGrandparentPair;

    queue <int> q;
    unordered_set<int> childrenHash;

    for (auto pair : childParentPair) {
        //push children to q
        q.push(pair.first);
        //hash children
        childrenHash.insert(pair.first);
    }

    while (!q.empty()) {

        //process each child
        int curr = q.front();
        q.pop();

        if (childrenHash.count(childParentPair[curr])) {

            queue<int> innerQ;
            innerQ.push(childParentPair[curr]);

            while (!innerQ.empty()) {
                int innerCurr = innerQ.front();
                innerQ.pop();
                //grandchildGrandparentPair.insert({ curr,childParentPair[innerCurr] });
                Database::insertAncestorRelation(curr, childParentPair[innerCurr]);

                if (childrenHash.count(childParentPair[innerCurr]))
                    innerQ.push(childParentPair[innerCurr]);
            }
        }
    }
}