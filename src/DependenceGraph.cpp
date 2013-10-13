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
    set<int> nodeSet;
     
    for(vector<Rule>::iterator it = _nlp.begin(); it != _nlp.end(); it++) {        
        if(it->type == RULE) {
            nodeSet.insert(it->head);
            for(vector<int>::iterator p_it = it->positive_literals.begin(); p_it != it->positive_literals.end(); p_it++) {
                dpdGraph[it->head].insert(*p_it);
                nodeSet.insert(*p_it);
            }
        }
    }
    
    nodeNumber = nodeSet.size();
    
    visit = new bool[nodeNumber + 1];
    memset(visit, false, sizeof(bool) * (nodeNumber + 1));
    DFN = new int[nodeNumber + 1];
    memset(DFN, 0, sizeof(int) * (nodeNumber + 1));
    Low = new int[nodeNumber + 1];
    memset(Low, 0, sizeof(int) * (nodeNumber + 1));

    loops.clear();
}

DependenceGraph::~DependenceGraph() {
    dpdGraph.clear();
    loops.clear();
    
    delete[] visit;
    delete[] DFN;
    delete[] Low;
}

void DependenceGraph::findSCC() {
    for(map<int, set<int> >::iterator it = dpdGraph.begin(); it != dpdGraph.end(); it++) {
        if(!visit[it->first]) {
            Index = 0;
            tarjan(it->first);
        }
    }
}

void DependenceGraph::tarjan(int u) {
    DFN[u] = Low[u] = ++Index;
    vs.push(u);
    visit[u] = true;
    for(set<int>::iterator it = dpdGraph[u].begin(); it != dpdGraph[u].end(); it++) {
        if(!visit[*it]) {
            tarjan(*it);
            if(Low[u] > Low[*it]) Low[u] = Low[*it];
        }
        else {
            if(Low[u] > DFN[*it]) Low[u] = DFN[*it];
        }
    }
    if(Low[u] == DFN[u]) {
        if(vs.top() != u) {
            Loop l;
            while(vs.top() != u) {                  
                l.loopNodes.insert(vs.top());
                vs.pop();
            }
            l.loopNodes.insert(u);
            vs.pop();
            SCCs.push_back(l);
        }
        else {
            vs.pop();
        }
    }
}

void DependenceGraph::findESRules() {
    for(vector<Loop>::iterator it = SCCs.begin(); it != SCCs.end(); it++) {
        int index = -1;
        for(vector<Rule>::iterator r = nlp.begin(); r != nlp.end(); r++) {
            index++;
            
            if(r->type == RULE) {
                if(Utils::inList(r->head, it->loopNodes) && 
                    !(Utils::crossList(r->positive_literals, it->loopNodes))/* && 
                    !(Utils::crossList(r->negative_literals, it->loopNodes)))*/) {
                    it->ESRules.push_back(index); 
                }
            }            
        }
    }
}

void DependenceGraph::printfLoop() {
    for(vector<Loop>::iterator it = SCCs.begin(); it != SCCs.end(); it++) {
        for(set<int>::iterator nit = it->loopNodes.begin(); nit != it->loopNodes.end(); nit++) {
            printf("%d ", *nit);
        }
        printf("\n");
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
    printf("loop es size %d ", loop.ESRules.size()); 
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
    findSCC();     //If use function DependenceGraph::test(), delete this line.
    findESRules();
       
    printf("The loops size : %d\n", SCCs.size());
    
    for(vector<Loop>::iterator it = SCCs.begin(); it != SCCs.end(); it++) {
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