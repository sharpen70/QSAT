#include <queue>

#include "RuleRepair.h"
#include "Vocabulary.h"
#include "ClakeCompletion.h"
using namespace std;

Repair::Repair(vector<Rule>& rules, vector<Rule>& qs) {
  this->choiceNLP = rules;
  this->queries = qs;
  
  this->choicePred = Vocabulary::instance().getSelAtoms();
}

_formula* Repair::computeQueryRule() {
  _formula* qf = NULL;
  
  for(size_t i = 0; i < this->queries.size(); i++) {
    Rule& r = this->queries[i];
    _formula* rf = Utils::convertRuleBodyToFormula(r);
    if(qf == NULL) qf = rf;
    else qf = Utils::compositeByConnective(DISJ, qf, rf);
  }
  
  return qf;
}

RepairQBF::RepairQBF(vector<Rule>& rules, vector<Rule>& qs):Repair(rules, qs) {  
  this->isomChoicePred = this->choicePred;
  for(size_t i = 0; i < this->isomChoicePred.size(); i++) {
    this->isomChoicePred[i] += Vocabulary::instance().apSize();
  }
}

//Preference Rule
//p1p2 < p1*p2* <==> (p1->p1*)CONJ(p2->p2*)CONJ~((p1*->p1)CONJ(p2*->p2))
_formula* RepairQBF::computePreferenceRule() {
  _formula* leq = NULL;
  _formula* neq = NULL;
  
  for(size_t i = 0; i < this->choicePred.size(); i++) {
    int p = this->choicePred[i];
    int ps = this->isomChoicePred[i];
    
    _formula* c1 = Utils::compositeByConnective(DISJ, Utils::compositeByConnective(NEGA,
              Utils::compositeToAtom(p)), Utils::compositeToAtom(ps));
    
    _formula* c2 = Utils::compositeByConnective(DISJ, Utils::compositeByConnective(NEGA,
              Utils::compositeToAtom(ps)), Utils::compositeToAtom(p));
    
    if(leq == NULL) leq = c1;
    else leq = Utils::compositeByConnective(CONJ, leq, c1);
    
    if(neq == NULL) neq = c2;
    else neq = Utils::compositeByConnective(CONJ, neq, c2);
  }
  
  neq = Utils::compositeByConnective(NEGA, neq);
  
  return Utils::compositeByConnective(CONJ, leq, neq);
}

//The Query entailment SAT formula
//EXIS(P)UNIV(P*)(A CONJ (~B DISJ ~C) CONJ ~Q)

//A = SM(Program) 
//B = P < P*
//C = SM(Program*)vi
//Q = Query
void RepairQBF::computeRepairQBFFormula(vector<_formula*>& SAT) {  
  ClakeCompletion CC(this->choiceNLP);
  
  queue<_formula*> aux;
  
  _formula* Af = CC.convertSyntaxTree();
//  printf("\nAF:\n");
//  Utils::formulaOutput(stdout, Af);
  _formula* Bf = this->computePreferenceRule();
//  printf("\nBF:\n");
//  Utils::formulaOutput(stdout, Bf);  
  _formula* Cf = Utils::copyIsomorFormula(Af, 1);
//  printf("\nCF:\n");
//  Utils::formulaOutput(stdout, Cf);
  Vocabulary::instance().newAux(Vocabulary::instance().apSize());
    
  _formula* Qf = this->computeQueryRule();
//    printf("\nQF:\n");
//  Utils::formulaOutput(stdout, Qf);
  
  _formula* QE;// = Utils::compositeByConnective(NEGA, Af);
  QE = Utils::compositeByConnective(DISJ, 
          Utils::compositeByConnective(NEGA, Bf), Utils::compositeByConnective(NEGA,
          Cf));
  QE = Utils::compositeByConnective(CONJ, Af, QE);
  QE = Utils::compositeByConnective(CONJ, QE, Utils::compositeByConnective(NEGA,
          Qf));

//  printf("\nQE:\n");
//  Utils::formulaOutput(stdout, QE);
  QE = CNFUtils::convertCNFWithAux(QE, aux);
  
  CNFUtils::divideCNFFormula(QE, SAT);
  
  while(!aux.empty()) {
    _formula* a = aux.front();
    aux.pop();
    a = CNFUtils::convertCNFWithAux(a, aux);
    CNFUtils::divideCNFFormula(a, SAT);
  }
}

RepairSAT::RepairSAT(vector<Rule>& rules, vector<Rule>& qs):Repair(rules, qs) {
  for(size_t i = 0; i < this->choiceNLP.size(); i++) {
    Rule r = this->choiceNLP[i];
    if(r.sel_pred != -1) {
      r.positive_literals.erase(r.sel_pred);
      if(r.positive_literals.size() == 0 && r.negative_literals.size() == 0)
        r.type = FACT;
      this->ruleDivision[r.sel_pred].push_back(r);
    }
  }
}

_formula* RepairSAT::computeDivision(int sel) {
  vector<Rule>& rs = this->ruleDivision[sel];
  _formula* p = Utils::compositeByConnective(NEGA, Utils::compositeToAtom(sel));
  _formula* df = NULL;
  for(size_t i = 0; i < rs.size(); i++) {
    Rule& r = rs[i];
    if(r.type == RULE) {
      _formula* body = Utils::convertRuleBodyToFormula(r);
      _formula* fml_l = Utils::compositeByConnective(DISJ, Utils::compositeByConnective(
              NEGA, Utils::compositeToAtom(r.head)), body);
      _formula* fml_r = Utils::compositeByConnective(DISJ, Utils::compositeByConnective(
              NEGA, Utils::copyFormula(body)), Utils::compositeToAtom(r.head));
      _formula* fml = Utils::compositeByConnective(CONJ, fml_l, fml_r);
      
      if(df == NULL) df = fml;
      else df = Utils::compositeByConnective(CONJ, df, fml);
    } 
    else if(r.type == CONSTRANT) {
      _formula* body = Utils::convertRuleBodyToFormula(r);
      _formula* fml = Utils::compositeByConnective(NEGA, body);
      
      if(df == NULL) df = fml;
      else df = Utils::compositeByConnective(CONJ, df, fml);
    }
    else if(r.type == FACT) {
      _formula* fml = Utils::compositeToAtom(r.head);
      
      if(df == NULL) df = fml;
      else df = Utils::compositeByConnective(CONJ, df, fml);
    }
  }
  
  if(df == NULL) return p;
  
  p = Utils::compositeByConnective(DISJ, p, Utils::compositeByConnective(NEGA,
          df));
  
  return p;
}

void RepairSAT::computeRepairSATFormula(vector<_formula*>& SAT) {
  ClakeCompletion CC(this->choiceNLP);
  
  _formula* Af = CC.convertSyntaxTree();
//  printf("\nAF:\n");
//  Utils::formulaOutput(stdout, Af);
  _formula* Qf = this->computeQueryRule();
//  printf("\nQF:\n");
//  Utils::formulaOutput(stdout, Qf);
  _formula* QE = Af;
//  _formula* QE = NULL;
  for(size_t i = 0; i < this->choicePred.size(); i++) {
    int sel = this->choicePred[i];
    _formula* p = this->computeDivision(sel);
    
    if(QE == NULL) QE = p;
    else QE = Utils::compositeByConnective(CONJ, QE, p);
  }
  
  QE = Utils::compositeByConnective(CONJ, QE, Utils::compositeByConnective(NEGA,
          Qf));
  
  queue<_formula*> aux;
 // Utils::formulaOutput(stdout, QE); printf("\n");
  QE = CNFUtils::convertCNFWithAux(QE, aux);
  
  CNFUtils::divideCNFFormula(QE, SAT);
  
  while(!aux.empty()) {
    _formula* a = aux.front();
    aux.pop();
    a = CNFUtils::convertCNFWithAux(a, aux);
    CNFUtils::divideCNFFormula(a, SAT);
  }
  
//  printf("\nSAT size %d\n", SAT.size());
}