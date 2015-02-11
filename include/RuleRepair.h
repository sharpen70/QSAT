/* 
 * File:   RuleRepairSAT.h
 * Author: sharpen
 *
 * Created on February 5, 2015, 1:58 PM
 */

#ifndef RULEREPAIRSAT_H
#define	RULEREPAIRSAT_H

#include <cstdlib>
#include <cstdio>
#include "ClakeCompletion.h"
#include "CNFUtils.h"
#include "Rule.h"

using namespace std;

class Repair {
public:
  Repair(vector<Rule>&, vector<Rule>&);
  vector<int> choicePred;
protected:
  vector<Rule> choiceNLP;
  vector<Rule> queries; 
  
  _formula* computeQueryRule();
};

class RepairQBF : public Repair {
public:
  RepairQBF(vector<Rule>&, vector<Rule>&);
  //The Query entailment SAT formula
  //EXIS(P)UNIV(P*)(A CONJ (~B DISJ ~C) CONJ Q)
  void computeRepairQBFFormula(vector<_formula*>&);
  vector<int> isomChoicePred;
private:    
  //Preference Rule
  //p1p2 < p1*p2* <==> (p1->p1*)CONJ(p2->p2*)CONJ~((p1*->p1)CONJ(p2*->p2))
  _formula* computePreferenceRule();
};

class RepairSAT : public Repair {
public:
  RepairSAT(vector<Rule>&, vector<Rule>&);
  void computeRepairSATFormula(vector<_formula*>&);
private:
  map<int, vector<Rule> > ruleDivision;
  _formula* computeDivision(int);
};
#endif	/* RULEREPAIRSAT_H */

