/* 
 * File:   DependenceGraph.h
 * Author: sharpen
 *
 * Created on September 3, 2013, 11:34 PM
 */

#ifndef DEPENDENCEGRAPH_H
#define	DEPENDENCEGRAPH_H

#include <vector>
#include <map>
#include <set>
#include <stack>
#include "Rule.h"
#include "Utils.h"

struct Loop {
    set<int> loopNodes;
    vector<int> ESRules;
    vector<_formula*> loopFormulas;
    
    Loop() {
        loopNodes.clear();
        loopFormulas.clear();
        ESRules.clear();
    }
    Loop(const Loop& l) {
        loopNodes = l.loopNodes;
        ESRules = l.ESRules;
        for(vector<_formula*>::const_iterator it = l.loopFormulas.begin(); 
                it != l.loopFormulas.end(); it++) {
            loopFormulas.push_back(Utils::copyFormula(*it));
        }
    }
    
    ~Loop() {
        for(vector<_formula*>::iterator it = loopFormulas.begin(); 
                it != loopFormulas.end(); it++) {
            Utils::deleteFormula(*it);
        }
        loopNodes.clear();
        loopFormulas.clear();
        ESRules.clear();
    }
};

class DependenceGraph {
public:
    DependenceGraph(vector<Rule> _dlp);
    DependenceGraph(const DependenceGraph& orig);
    ~DependenceGraph();
//    void test();                //nonsense, just for test
    
    vector<_formula*> computeLoopFormulas(Loop loop);
    void operateGraph();
    vector<Loop> getLoopWithESRuleSize(int k);
    vector<int> getESRSizes();
    
    vector<Loop> loops;
    vector<Loop> SCCs;
    void findSCC();
    
    void printfLoop();
private:
    vector<Rule> nlp;
    map<int, set<int> > dpdGraph;
    
    map<int, vector<Loop> > loopWithESSize;
    
    int nodeNumber;
    void findESRules();
    
    void tarjan(int u);

    //SCC
    bool *visit;
    int *DFN;
    int *Low;
    int Index;
    stack<int> vs;
};

#endif	/* DEPENDENCEGRAPH_H */
