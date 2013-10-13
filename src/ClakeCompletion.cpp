#include "ClakeCompletion.h"
#include "CNFUtils.h"
#include "NNFUtils.h"
#include "Utils.h"

ClakeCompletion::ClakeCompletion() {
    no_ipf_atoms.clear();
    constrants.clear();
    ipf_atoms_rules.clear();   
}

ClakeCompletion::~ClakeCompletion() {
    no_ipf_atoms.clear();
    constrants.clear();
    ipf_atoms_rules.clear();  
}

ClakeCompletion& ClakeCompletion::instance() {
    static ClakeCompletion theInstance;
    return theInstance;
}

vector<_formula*> ClakeCompletion::convert() {
    vector<_formula*> completion;
    
    for(map<int, vector<Rule> >::iterator it = ipf_atoms_rules.begin(); it != 
            ipf_atoms_rules.end(); it++) {
        vector<Rule> ipf_rules = it->second;
        _formula* tl = Utils::compositeToAtom(it->first);
        _formula* tr = NULL;
        
        for(vector<Rule>::iterator it = ipf_rules.begin(); it != ipf_rules.end();
                it++) {
            
            if(it->body_length == 0) {
                tr = NULL;
                break;
            }
            
            _formula* fr = Utils::convertRuleBodyToFormula(*it);
            
            if(fr == NULL) break;
            if(tr == NULL) {
                tr = fr;
            }
            else {
                tr = Utils::compositeByConnective(DISJ, tr, fr);
            }
        }
        
        if(tr != NULL) {
            _formula* ntl = Utils::compositeByConnective(NEGA, Utils::copyFormula(tl));
            _formula* ntr = Utils::compositeByConnective(NEGA, Utils::copyFormula(tr));
            _formula* impl_l = Utils::compositeByConnective(DISJ, ntl, tr);
            _formula* impl_r = Utils::compositeByConnective(DISJ, ntr, tl);       
            
            
            completion = Utils::joinFormulas(completion, CNFUtils::convertCNF(impl_l));
            completion = Utils::joinFormulas(completion, CNFUtils::convertCNF(impl_r));
        }
        else {
            completion.push_back(tl);
        }
    }
    
    for(vector<int>::iterator it = no_ipf_atoms.begin(); it != no_ipf_atoms.end();
            it++) {
        _formula* nega_atom = Utils::compositeByConnective(NEGA, Utils::compositeToAtom(
                *it));
        
        completion.push_back(nega_atom);
    }
    for(vector<Rule>::iterator it = constrants.begin(); it != constrants.end(); 
            it++) {
        _formula* fc = Utils::convertRuleBodyToFormula(*it);     
        
        fc = Utils::compositeByConnective(NEGA, fc);
        fc = NNFUtils::convertToNegativeNormalForm(fc);
        vector<_formula*> joinf;
        joinf.push_back(fc);
        completion = Utils::joinFormulas(completion, joinf);
    }
    return completion;
}

void ClakeCompletion::setDlp(const vector<Rule> nlp) {
    vector<Rule> _nlp = nlp;
    for(vector<Rule>::iterator it = _nlp.begin(); it != _nlp.end(); it++) {
        int a = it->head;
        
        if(a > 0) {
            ipf_atoms_rules[a].push_back(*it);
        }
        else {
            constrants.push_back(*it);
        }
    }
    
    for(int i = 1; i <= Vocabulary::instance().apSize(); i++) {
        bool in = false;
        
        for(map<int, vector<Rule> >::iterator it = ipf_atoms_rules.begin(); it != ipf_atoms_rules.end(); it++) {
            if(it->first == i) {
                in = true;
            }
        }
        
        if(!in) {
            no_ipf_atoms.push_back(i);
        }
    } 
}

void ClakeCompletion::testCompletion() {
    vector<_formula*> completion = convert();
    vector< set<int> > res = Utils::convertToSATInput(completion);
    
    for(vector<set <int> >::iterator it = res.begin(); it != res.end(); it++) {
        for(set<int>::iterator s_it = it->begin(); s_it != it->end(); s_it++) {
            printf("%d ", *s_it);
        }
        printf("\n");
    }
}
void ClakeCompletion::test() {
    printf("\nno_ipf_atoms:");
    for(int i = 0; i < no_ipf_atoms.size(); i++) {
        printf("%s ", Vocabulary::instance().getAtom(no_ipf_atoms.at(i)));
    }
    printf("\nipf_atoms_rules:\n");
    for(map<int, vector<Rule> >::iterator it = ipf_atoms_rules.begin(); it != ipf_atoms_rules.end(); it++) {
        vector<Rule> r = it->second;
        for(int i = 0; i < r.size(); i++) {
            r.at(i).output(stdout);
        }
    }
    printf("\nconstrants\n");
    for(int i = 0; i < constrants.size(); i++) {
        constrants.at(i).output(stdout);
    }
    
    testCompletion();
}