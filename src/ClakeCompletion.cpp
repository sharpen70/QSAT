#include "ClakeCompletion.h"
#include "CNFUtils.h"
#include "NNFUtils.h"
#include "Utils.h"

ClakeCompletion::ClakeCompletion(const vector<Rule>& nlp) {
  no_ipf_atoms.clear();
  constrants.clear();
  ipf_atoms_rules.clear();
  
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
    
    if(ipf_atoms_rules.find(i) == ipf_atoms_rules.end())
      no_ipf_atoms.push_back(i);
    //    for(map<int, vector<Rule> >::iterator it = ipf_atoms_rules.begin(); it != ipf_atoms_rules.end(); it++) {
    //      if(it->first == i) {
    //        in = true;
    //      }
    //    }
    //    
    //    if(!in) {
    //      no_ipf_atoms.push_back(i);
    //    }
  } 
}

ClakeCompletion::~ClakeCompletion() {
  no_ipf_atoms.clear();
  constrants.clear();
  ipf_atoms_rules.clear();  
}

//vector<_formula*> ClakeCompletion::convert() {
//  vector<_formula*> completion;
//  
//  for(map<int, vector<Rule> >::iterator it = ipf_atoms_rules.begin(); it != 
//          ipf_atoms_rules.end(); it++) {
//    vector<Rule> ipf_rules = it->second;
//    _formula* tl = Utils::compositeToAtom(it->first);
//    _formula* tr = NULL;
//    
//    for(vector<Rule>::iterator it = ipf_rules.begin(); it != ipf_rules.end();
//            it++) {
//      
//      if(it->type == FACT) {
//        tr = NULL;
//        break;
//      }
//      
//      _formula* fr = Utils::convertRuleBodyToFormula(*it);
//      
//      if(fr == NULL) break;
//      if(tr == NULL) {
//        tr = fr;
//      }
//      else {
//        tr = Utils::compositeByConnective(DISJ, tr, fr);
//      }
//    }
//    
//    if(tr != NULL) {
//      _formula* ntl = Utils::compositeByConnective(NEGA, Utils::copyFormula(tl));
//      _formula* ntr = Utils::compositeByConnective(NEGA, Utils::copyFormula(tr));
//      _formula* impl_l = Utils::compositeByConnective(DISJ, ntl, tr);
//      _formula* impl_r = Utils::compositeByConnective(DISJ, ntr, tl);       
//      
//      
//      Utils::joinFormulas(completion, CNFUtils::convertCNFSet(impl_l));
//      Utils::joinFormulas(completion, CNFUtils::convertCNFSet(impl_r));
//    }
//    else {
//      completion.push_back(tl);
//    }
//  }
//  
//  for(vector<int>::iterator it = no_ipf_atoms.begin(); it != no_ipf_atoms.end();
//          it++) {
//    _formula* nega_atom = Utils::compositeByConnective(NEGA, Utils::compositeToAtom(
//            *it));
//    
//    completion.push_back(nega_atom);
//  }
//  for(vector<Rule>::iterator it = constrants.begin(); it != constrants.end(); 
//          it++) {
//    _formula* fc = Utils::convertRuleBodyToFormula(*it);     
//    
//    fc = Utils::compositeByConnective(NEGA, fc);
//    fc = NNFUtils::convertToNegativeNormalForm(fc);
//    vector<_formula*> joinf;
//    joinf.push_back(fc);
//    Utils::joinFormulas(completion, joinf);
//  }
//  return completion;
//}

_formula* ClakeCompletion::convertSyntaxTree() {
  _formula* syntaxTree = NULL;
  
  for(map<int, vector<Rule> >::iterator it = ipf_atoms_rules.begin(); it != 
          ipf_atoms_rules.end(); it++) {
    vector<Rule>& ipf_rules = it->second;
    _formula* tl = Utils::compositeToAtom(it->first);
    _formula* tr = NULL;
    
    for(vector<Rule>::iterator it = ipf_rules.begin(); it != ipf_rules.end();
            it++) {
      
      if(it->type == FACT) {
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
      
      _formula* impl = Utils::compositeByConnective(CONJ, impl_l, impl_r);
      
      if(syntaxTree == NULL) syntaxTree = impl;
      else syntaxTree = Utils::compositeByConnective(CONJ, syntaxTree, impl);
    }
    else {
      if(syntaxTree == NULL) syntaxTree = tl;
      else syntaxTree = Utils::compositeByConnective(CONJ, syntaxTree, tl);
    }
  }
  
  for(vector<int>::iterator it = no_ipf_atoms.begin(); it != no_ipf_atoms.end();
          it++) {
    _formula* nega_atom = Utils::compositeByConnective(NEGA, Utils::compositeToAtom(
            *it));
    if(syntaxTree == NULL) syntaxTree = nega_atom;
    else syntaxTree = Utils::compositeByConnective(CONJ, syntaxTree, nega_atom);
  }
  
  for(vector<Rule>::iterator it = constrants.begin(); it != constrants.end(); 
          it++) {
    _formula* fc = Utils::convertRuleBodyToFormula(*it);     
    
    fc = Utils::compositeByConnective(NEGA, fc);
  //  fc = CNFUtils::convertToNegativeNormalForm(fc);
    
    if(syntaxTree == NULL) syntaxTree = fc;
    else syntaxTree = Utils::compositeByConnective(CONJ, syntaxTree, fc);
  }
  
  return syntaxTree;  
}