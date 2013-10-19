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
            for(set<int>::iterator p_it = it->positive_literals.begin(); p_it != it->positive_literals.end(); p_it++) {
                dpdGraph[it->head].insert(*p_it);
                nodeSet.insert(*p_it);
            }
        }
    }
    
    nodeNumber = nodeSet.size();
    maxNode = *(--nodeSet.end());
    visit = new bool[maxNode + 1];
    memset(visit, false, sizeof(bool) * (maxNode + 1));
    DFN = new int[maxNode + 1];
    memset(DFN, 0, sizeof(int) * (maxNode + 1));
    Low = new int[maxNode + 1];
    memset(Low, 0, sizeof(int) * (maxNode + 1));
    involved = new bool[maxNode + 1];
    memset(involved, true, sizeof(bool) * (maxNode + 1));
}

DependenceGraph::~DependenceGraph() {
    dpdGraph.clear();
    
    delete[] visit;
    delete[] DFN;
    delete[] Low;
}

vector<Loop> DependenceGraph::findSCC() {
    vector<Loop> loops;
    memset(visit, false, sizeof(bool) * (maxNode + 1));

    for(map<int, set<int> >::iterator it = dpdGraph.begin(); it != dpdGraph.end(); it++) {
        if(!visit[it->first] && involved[it->first]) {
            Index = 0;
            tarjan(it->first, loops);
        }
    }
    
    return loops;
}

void DependenceGraph::tarjan(int u, vector<Loop>& loops) {
    DFN[u] = Low[u] = ++Index;
    vs.push(u);
    visit[u] = true;
    for(set<int>::iterator it = dpdGraph[u].begin(); it != dpdGraph[u].end(); it++) {
        if(!involved[*it]) continue;
        
        if(!visit[*it]) {
            tarjan(*it, loops);
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
            loops.push_back(l);
        }
        else {
            vs.pop();
        }
    }
}

void DependenceGraph::findESRules(Loop& loop) {
    int index = -1;
    
    for(vector<Rule>::iterator r = nlp.begin(); r != nlp.end(); r++) {
        index++;

        if(r->type == RULE) {
            if(loop.loopNodes.find(r->head) != loop.loopNodes.end() && 
                !(Utils::crossSet(r->positive_literals, loop.loopNodes))/* && 
                !(Utils::crossList(r->negative_literals, it->loopNodes)))*/) {
                loop.ESRules.insert(index); 
            }
        }            
    }    
}

void DependenceGraph::findAllESRules() {
    for(vector<Loop>::iterator it = SCCs.begin(); it != SCCs.end(); it++) {
        findESRules(*it);
    }
}

void DependenceGraph::printfLoop() {
    for(vector<Loop>::iterator it = SCCs.begin(); it != SCCs.end(); it++) {
        for(set<int>::iterator nit = it->loopNodes.begin(); nit != it->loopNodes.end(); nit++) {
            printf("%d %s", *nit, Vocabulary::instance().getAtom(*nit));
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

int DependenceGraph::computeLoopFormulas(Loop& loop) {
    _formula* _head = NULL;
    vector<_formula*> result;
    int newVar = 0;
        
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
    for(set<int>::iterator rit = loop.ESRules.begin(); rit != loop.ESRules.end();
            rit++) {
        char newAtom[MAX_ATOM_LENGTH];
        sprintf(newAtom, "Rule_%d", *rit);
        int id = Vocabulary::instance().queryAtom(newAtom);
        if(id < 0) {
            newVar++;
            id = Vocabulary::instance().addAtom(strdup(newAtom));
            _formula* rule = Utils::convertRuleBodyToFormula(nlp.at(*rit));
            _formula* nega = Utils::compositeByConnective(NEGA, 
                    Utils::copyFormula(rule));
            _formula* l1 = Utils::compositeByConnective(DISJ, nega,
                    Utils::compositeToAtom(id));
            _formula* l2 = Utils::compositeByConnective(DISJ, rule,
                    Utils::compositeByConnective(NEGA, Utils::compositeToAtom(id)));
            Utils::joinFormulas(loop.loopFormulas, CNFUtils::convertCNF(l1));
            Utils::joinFormulas(loop.loopFormulas, CNFUtils::convertCNF(l2));
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

    loop.loopFormulas.push_back(lf); 
    
    return newVar;
} 

vector<Loop> DependenceGraph::findCompMaximal(set<int> comp) {
    vector<Loop> maximals;
    
    memset(involved, false, sizeof(bool) * (maxNode + 1));
    for(set<int>::iterator it = comp.begin(); it != comp.end(); it++) {
        involved[*it] = true;
    }
    vector<Loop> sccs = findSCC();
    for(vector<Loop>::iterator it = sccs.begin(); it != sccs.end(); it++) {
        findESRules(*it); 
        maximals.push_back(findLoopMaximal(*it));
    }
    
    return maximals;
    //return sccs;
}

Loop DependenceGraph::findLoopMaximal(Loop scc) {      
    if(scc.ESRules.size() == 0) return scc;
    
    memset(involved, false, sizeof(bool) * (maxNode + 1));
    
    for(set<int>::iterator it = scc.loopNodes.begin(); it != scc.loopNodes.end(); it++) {
        involved[*it] = true;      
    }  
    
    int tempR = 0;
    
    for(set<int>::iterator it = scc.ESRules.begin(); it != scc.ESRules.end(); it++) {
        int r = nlp.at(*it).head;
        involved[tempR] = true;
        involved[r] = false;
        tempR = r;
        
        vector<Loop> sccs = findSCC();
        if(sccs.size() == 0) continue;  
        else {
            for(vector<Loop>::iterator sit = sccs.begin(); sit != sccs.end(); sit++) {
                findESRules(*sit);
                set<int> moreoverRules;
                for(set<int>::iterator es = sit->ESRules.begin(); es != sit->ESRules.end();
                        es++) {
                    if(scc.ESRules.find(*es) == scc.ESRules.end()) {
                        moreoverRules.insert(*es);
                    }
                }
                if(moreoverRules.size() == 0) {
                    return findLoopMaximal(*sit);
                }
                else {
                    memset(involved, false, sizeof(bool) * (maxNode + 1));
    
                    for(set<int>::iterator lit = sit->loopNodes.begin(); 
                            lit != sit->loopNodes.end(); lit++) {
                        involved[*lit] = true;      
                    } 
                    for(set<int>::iterator mit = moreoverRules.begin(); 
                            mit != moreoverRules.end(); mit++) {
                        involved[nlp.at(*mit).head] = false;      
                    }
                    
                    vector<Loop> mscc = findSCC();
                    if(mscc.size() != 0) {
                        findESRules(mscc.front());
                        return findLoopMaximal(mscc.front());
                    }
                }
            }
        }
    }
   
    return scc;    
}

void DependenceGraph::operateGraph() {
    SCCs = findSCC();     //If use function DependenceGraph::test(), delete this line.
    findAllESRules();
       
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