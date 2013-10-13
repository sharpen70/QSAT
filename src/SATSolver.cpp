/* 
 * File:   SATSolver.cpp
 * Author: sharpen
 * 
 * Created on September 4, 2013, 9:04 PM
 */

#include "SATSolver.h"
#include "Vocabulary.h"

SATSolver::SATSolver(vector< set<int> > cnf, int num_lits) {
    num_lits_in_NLP = num_lits;
    
    while(num_lits > sat.nVars) {
        sat.newVar();
    }
    
    for(vector< set<int> >::iterator it = cnf.begin(); it != cnf.end(); it++) {
        int var;
        vec<Lit> lits;
        
        for(set<int>::iterator l_it = it->begin(); l_it != it->end(); l_it++) {
            var = abs(*l_it) - 1;
            lits.push((*l_it > 0) ? Lit(var) : ~Lit(var));
        }
        
        if(lits.size() == 1) {
            if(!sat.addUnit(lits[0])) {
                printf("addUnit failed");
            }
        }
        else {
            if(!sat.addClause(lits)) {
                printf("addClause failed");
            }
        }
    }
}

SATSolver::~SATSolver() {
}

int SATSolver::invokeSAT() {
    int i = 0;
    while(isExistModel())
        i++;
    return i;
}

bool SATSolver::isExistModel() {
    bool res;
    set<int> model_set;
    vec<Lit> new_clause;
    
    res = sat.solve();
    
    if(res) {
        for(int i = 0; i < num_lits_in_NLP; i++) {
            if(sat.model[i]) {
                model_set.insert(i + 1);
                new_clause.push(~Lit(i));
            }
            else {
                new_clause.push(Lit(i));
            }
        }        
        models.push_back(model_set);
        
//        for(set<int>::iterator it = model_set.begin(); it != model_set.end(); it++) {
//            printf("%d ", *it);
//        }
//        printf("\n");
        if(!sat.addClause(new_clause)) {
            return false;
        }
        return true;
    }
    else {
        return false;
    }
}

void SATSolver::outputResult() {
    int i = 1;
    
    for(vector< set<int> >::iterator s_it = models.begin(); s_it != models.end();
            s_it++) {
        printf("Model %d:\n", i);
        i++;
        for(set<int>::iterator m_it = s_it->begin(); m_it != s_it->end(); m_it++) {
            printf("%s", Vocabulary::instance().getAtom(*m_it));
        }
        printf("\n");
    }
}