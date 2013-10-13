/* 
 * File:   DependenceGraph.cpp
 * Author: sharpen
 * 
 * Created on September 3, 2013, 11:34 PM
 */

#include "DependenceGraph.h"
#include "Utils.h"
#include <cstring>
#include <assert.h>
#include <vector>
#include "Rule.h"
#include <map>
#include "structs.h"
#include "Vocabulary.h"
#include "CNFUtils.h"
#include <string.h>
#include <set>
#include <algorithm>
#include <functional>

DependenceGraph::DependenceGraph(vector<Rule> _nlp) :
                nlp(_nlp) {
    map< pair<int, int>, bool> graph;
    set<int> nodeSet;
     
    for(vector<Rule>::iterator it = _nlp.begin(); it != _nlp.end(); it++) {        
        if(it->head > 0 && it->positive_literals.size() > 0) {
            nodeSet.insert(it->head);
            for(vector<int>::iterator p_it = it->positive_literals.begin(); p_it != it->positive_literals.end(); p_it++) {
                dpdGraph[it->head].insert(*p_it);
                nodeSet.insert(*p_it);
            }
        }
    }
    
    FILE* out = fopen("res/output/sample.out", "w");
    
    edgePointer = 0;
    nodeNumber = *(--nodeSet.end());
    edgeNumber = 0;
    
    fprintf(out, "%d ", nodeNumber);
  
    
    for(map<int, set<int> >::iterator it = dpdGraph.begin(); it != dpdGraph.end(); it++) {
        edgeNumber += it->second.size();
    }
    
    fprintf(out, "%d\n", edgeNumber);
    
    heads = new int[nodeNumber + 1];
    memset(heads, -1, sizeof(int) * (nodeNumber + 1));

    edges = new Edge[edgeNumber];
    memset(edges, -1, sizeof(Edge) * edgeNumber);
    
    for(map<int, set<int> >::iterator it = dpdGraph.begin(); it != dpdGraph.end(); it++) {
        for(set<int>::iterator p_it = it->second.begin(); p_it != it->second.end(); p_it++) {
            pair<int, int> temp = make_pair(it->first, *p_it);
            if(graph[temp] != true) {
                graph[temp] = true;
                addEdge(it->first, *p_it);
                fprintf(out, "%d %d\n", it->first, *p_it);
            }           
        }
    }
                   
    
    loops.clear();
}

DependenceGraph::~DependenceGraph() {
    dpdGraph.clear();
    loops.clear();
    
    nodeNumber = edgeNumber = edgePointer = 0;

    if (heads) {
        delete[] heads;
        heads = NULL;
    }

    if (edges) {
        delete[] edges;
        edges = NULL;
    }
}

void DependenceGraph::addEdge(int x, int y) {
    edges[edgePointer].x = x;
    edges[edgePointer].y = y;
    edges[edgePointer].next = heads[x];
    heads[x] = edgePointer ++;
}

void DependenceGraph::dfs(int depth, int x, Info &info) {
    int *path = info.path;
    bool *vis = info.vis;
    
    if (x < info.startPoint) return ;
    
    path[depth] = x;

    if (depth > 0 && x == info.startPoint) {
        Hash hash(path, depth);
        if (!info.m[hash]) {
            info.m[hash] = true;
            Loop loop;
            for (int i = 0; i < depth; i ++) {
                loop.loopNodes.insert(path[i]);
            }
            Hash lhash(loop.loopNodes);
            if(!loopHash[lhash]) {
                loopHash[lhash] = true;
                loops.push_back(loop);
            }
        }
    }

    for (int i = heads[x]; i != -1; i = edges[i].next) {
        if (!vis[i]) {
            vis[i] = true;
            dfs(depth + 1, edges[i].y, info);
            vis[i] = false;
        }
    }
}

void DependenceGraph::find() {
    Info info;

    info.path = new int[edgeNumber];
    memset(info.path, -1, sizeof(int) * edgeNumber);

    info.vis = new bool[edgeNumber];
    memset(info.vis, false, sizeof(bool) * edgeNumber);

    for (int i = 1; i <= nodeNumber; i ++) {
        info.startPoint = i;
        dfs(0, info.startPoint, info);
    }
}

void DependenceGraph::findESRules() {
    for(vector<Loop>::iterator it = loops.begin(); it != loops.end(); it++) {
        int index = -1;
        for(vector<Rule>::iterator r = nlp.begin(); r != nlp.end(); r++) {
            index++;
            
            if(r->body_length == 0) {
                continue;
            }
            
            if(Utils::inList(r->head, it->loopNodes) && 
                    (!(Utils::crossList(r->positive_literals, it->loopNodes)) && 
                    !(Utils::crossList(r->negative_literals, it->loopNodes)))) {
                it->ESRules.push_back(index);              
            }
        }
    }
}


//void DependenceGraph::test() {   
//    printf("\nThe dependence graph :\n");
//    printf("\nThe nlp : %d\n", nlp.size());
//    for(vector<Rule>::iterator it = nlp.begin(); it != nlp.end(); it++) {
//        (*it).output(stdout);
//    }
//      
//    printf("\nThe dpdGraph :\n");
//    for(map<int, set<int> >::iterator mit  = dpdGraph.begin(); 
//            mit != dpdGraph.end(); mit++) {
//        printf("The head : %s,   The body : ", Vocabulary::instance().getAtom(mit->first));
//        for(set<int>::iterator bodyit = mit->second.begin();
//                bodyit != mit->second.end(); bodyit++) {
//            printf("%s, ", Vocabulary::instance().getAtom(*bodyit));
//        }
//        printf("\n");
//    }
//    fflush(stdout);
//    find();
//    
//    computeLoopFormulas();
//    printf("\nThe loops :\n");
//    
//    int i = 1;
//    for(vector<Loop>::iterator it = loops.begin(); 
//            it != loops.end(); it++) {
//        printf("L%d : ", i++);
//        for(set<int>::iterator lit = it->loopNodes.begin(); 
//                lit != it->loopNodes.end(); lit++) {
//            printf("%s, ", Vocabulary::instance().getAtom(*lit));
//        }
//        printf("\n");
//        printf("ESupport: \n");
//        for(vector<int>::iterator rit = it->ESRules.begin(); rit != it->ESRules.end();
//                rit++) {
//            nlp.at(*rit).output(stdout);
//        }
//        printf("LoopFormula: \n");
//        for(vector<_formula*>::iterator fit = it->loopFormulas.begin();
//                fit != it->loopFormulas.end(); fit ++) {
//            Utils::formulaOutput(stdout, *fit);
//        }
//        printf("\n");
//    }
////    
////    
////    //-----------------LOOP FORMULA-----------------------------
////    vector<_formula*> lf = computeLoopFormulas();
////    printf("\nThe loop formulas :\n");
////    for(vector<_formula*>::iterator it = lf.begin(); it != lf.end(); it++) {
////        Utils::formulaOutput(stdout, *it);
////        printf("\n");
////    }
////    
////   printf("\nLF to sat input :\n");
////    //convert to CNF for the input
////    vector<_formula*> input;
////    for(vector<_formula*>::iterator it = lf.begin(); it != lf.end(); it++) {
////        vector<_formula*> cnflf = CNFUtils::convertCNF(*it);
////        input = Utils::joinFormulas(input, cnflf);
////    }
////    for(vector<_formula*>::iterator it = input.begin(); it != input.end(); it++) {
////        Utils::formulaOutput(stdout, *it);
////        printf("\n");
////    }
////    
////    vector< set<int> > satInput = Utils::convertToSATInput(input);
////    for(vector< set<int> >::iterator siit = satInput.begin(); 
////            siit != satInput.end(); siit++) {
////        for(set<int>::iterator sit = (*siit).begin(); 
////                sit!= (*siit).end(); sit++) {
////            printf("%d ", *sit);
////        }
////        printf("\n");
////    }
////    
////    //extendSupportWithSize
////    printf("\nThe Extend support with size : total %d\n", extendSupportRulesWithSize.size());
////    for(map<int, vector<_formula*> >::iterator it = extendSupportRulesWithSize.begin();
////            it != extendSupportRulesWithSize.end(); it++) {
////        printf("size %d :\n", it->first);
////        for(vector<_formula*>::iterator vit = it->second.begin();
////                vit != it->second.end(); vit++) {
////            Utils::formulaOutput(stdout, *vit);
////            printf("\n");
////        }
////    }
//}

vector<_formula*> DependenceGraph::computeLoopFormulas(Loop loop) {
    _formula* _head = NULL;
    vector<_formula*> result;
        
    for(set<int>::iterator hit = loop.loopNodes.begin(); hit != loop.loopNodes.end();
            hit++) {
        if(_head == NULL) _head = Utils::compositeByConnective(NEGA,
                Utils::compositeToAtom(*hit));
        else {
            _formula* newhead = Utils::compositeByConnective(NEGA,
                Utils::compositeToAtom(*hit));
            _head = Utils::compositeByConnective(DISJ, _head, newhead);
        }
    }
    _formula* _body = NULL;
    printf("loop es size%d", loop.ESRules.size()); fflush(stdout);
    for(vector<int>::iterator rit = loop.ESRules.begin(); rit != loop.ESRules.end();
            rit++) {
        char newAtom[MAX_ATOM_LENGTH];
        sprintf(newAtom, "Rule_%d", *rit);
        int id = Vocabulary::instance().queryAtom(newAtom);
        if(id < 0) {
            id = Vocabulary::instance().addAtom(strdup(newAtom));
            _formula* rule = Utils::convertRuleBodyToFormula(nlp.at(*rit));
            _formula* nega = Utils::compositeByConnective(NEGA, 
                    Utils::copyFormula(rule));
            _formula* l1 = Utils::compositeByConnective(DISJ, nega,
                    Utils::compositeToAtom(id));
            _formula* l2 = Utils::compositeByConnective(DISJ, rule,
                    Utils::compositeByConnective(NEGA, Utils::compositeToAtom(id)));
            result = Utils::joinFormulas(result, 
                    CNFUtils::convertCNF(l1));
            result = Utils::joinFormulas(result, 
                    CNFUtils::convertCNF(l2));
        }

        if(_body == NULL) {
            _body = Utils::compositeToAtom(id);
        } 
        else {
            _body = Utils::compositeByConnective(DISJ, _body, Utils::compositeToAtom(id));
        }
    }
    _formula* lf;
    if(_body == NULL) lf = _head;
    else lf = Utils::compositeByConnective(DISJ, _head, _body);

    result.push_back(lf); 
    
    return result;
} 

void DependenceGraph::operateGraph() {
    find();     //If use function DependenceGraph::test(), delete this line.
    findESRules();
       
    printf("The loops size : %d\n", loops.size());
    
    for(vector<Loop>::iterator it = loops.begin(); it != loops.end(); it++) {
       
        
//        loopWithESSize[it->ESRules.size()] = Utils::joinFormulas(
//                loopWithESSize[it->ESRules.size()], it);
        loopWithESSize[it->ESRules.size()].push_back(*it);
    }
}


vector<Loop> DependenceGraph::getLoopWithESRuleSize(int k) {
    return this->loopWithESSize[k];
}

vector<int> DependenceGraph::getESRSizes() {
    vector<int> result;
    
    for(map<int, vector<Loop> >::iterator it = loopWithESSize.begin();
            it != loopWithESSize.end(); it++) {
        result.push_back(it->first);
    }
    
    sort(result.begin(), result.end());    //default comp is operator < 
    
    return result;
}