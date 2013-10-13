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

/*struct Edge {
    int x, y;
    int next;
};

struct Hash {
    int sum, squareSum;

    friend bool operator <(const Hash &a, const Hash &b) {
        if (a.sum != b.sum) {
            return a.sum < b.sum;
        }
        else {
            return a.squareSum < b.squareSum;
        }
    }

    Hash() {
        sum = squareSum = 0;
    }

    Hash(int *array, int size) {
        sum = getSum(array, size);
        squareSum = getSquareSum(array, size);
    }
    
    Hash(set<int> array) {
        sum = getSum(array);
        squareSum = getSquareSum(array);
    }
    
    int getSum(int *array, int size) {
        int res = 0;
        for (int i = 0; i < size; i ++) {
            res += array[i];
        }
        return res;
    }
    
    int getSum(set<int> array) {
        int res = 0;
        for (set<int>::iterator it = array.begin(); it != array.end(); it++) {
            res += *it;
        }
        return res;
    }
    
    int getSquareSum(set<int> array) {
        int res = 0;
        for (set<int>::iterator it = array.begin(); it != array.end(); it++) {
            res += (*it) * (*it);
        }
        return res;
    }
    
    int getSquareSum(int* array, int size) {
        int res = 0;
        for (int i = 0; i < size; i ++) {
            res += array[i] * array[i];
        }
        return res;
    }
};

struct Info {
    int startPoint;
    int *path;
    bool *vis;
    map<Hash, bool> m;

    Info():startPoint(0), path(NULL), vis(NULL) {
        m.clear();
    };
    ~Info() {
        startPoint = 0;
        m.clear();
        if (path) {
            delete[] path; path = NULL;
        }
        if (vis) {
            delete[] vis; vis = NULL;
        }
    }
};*/

class DependenceGraph {
public:
    DependenceGraph(vector<Rule> _dlp);
    DependenceGraph(const DependenceGraph& orig);
    ~DependenceGraph();
//    void find();
//    void test();                //nonsense, just for test
    
    vector<_formula*> computeLoopFormulas(Loop loop);
    void operateGraph();
    vector<Loop> getLoopWithESRuleSize(int k);
    vector<int> getESRSizes();
    
    vector<Loop> loops;
    vector<Loop> SCCs;
    void findSCC();
    
private:
    vector<Rule> nlp;
    map<int, set<int> > dpdGraph;
    
    map<int, vector<Loop> > loopWithESSize;
    
    void findESRules();
    void addEdge(int x, int y);
 //   void dfs(int depth, int x, Info &info);
    
    void tarjan(int u);
    
    int nodeNumber, edgeNumber;
    int edgePointer;
    int *heads;
 //   Edge *edges;
 //   map<Hash, bool> loopHash;
    
    //SCC
    bool *visit;
    int *DFN;
    int *Low;
    int Index;
    stack<int> vs;
};

#endif	/* DEPENDENCEGRAPH_H */
