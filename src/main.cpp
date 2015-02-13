/* 
 * File:   main.cpp
 * Author: yzf
 *
 * Created on July 10, 2013, 2:17 PM
 */
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <assert.h>
#include "Vocabulary.h"
#include "structs.h"
#include "ClakeCompletion.h"
#include "CNFUtils.h"
#include "RuleRepair.h"
#include <set>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

FILE* fout;
extern FILE* yyin;
extern vector<Rule> G_NLP;
extern vector<Rule> Queries;
extern int yyparse();

void io(const char* iPathName, const char* oPathName) {
  yyin = fopen (iPathName, "r");
  fout = fopen (oPathName, "w+");
  
  if (! yyin) {
    printf("IO Error: cannot open the input file.\n" );
    assert(0);
  }
  if (! fout) {
    printf("IO Error: cannot open the output file.\n");
    assert(0);
  }
}

void testCNF() {
  Vocabulary::instance().addMapAtom("a");
  Vocabulary::instance().addMapAtom("b");
  Vocabulary::instance().addMapAtom("c");
  Vocabulary::instance().addMapAtom("d");
  
  _formula* a = Utils::compositeToAtom(1);
  _formula* b = Utils::compositeToAtom(2);
  _formula* c = Utils::compositeToAtom(3);
  _formula* d = Utils::compositeToAtom(4);
  
  _formula* ac = Utils::compositeByConnective(DISJ, a, c);
  _formula* bc = Utils::compositeByConnective(DISJ, b, c);
  _formula* ad = Utils::compositeByConnective(DISJ, a, d);
  _formula* bd = Utils::compositeByConnective(DISJ, b, d);
  
  _formula* nac = Utils::compositeByConnective(NEGA, ac);
  _formula* nbc = Utils::compositeByConnective(NEGA, bc);
  _formula* cnac = Utils::compositeByConnective(CONJ, nac, nbc);
  _formula* acbc = Utils::compositeByConnective(CONJ, ac, bc);
  _formula* adbd = Utils::compositeByConnective(CONJ, ad, bd);
  _formula* abcd = Utils::compositeByConnective(DISJ, acbc, adbd);

  printf("before CNF\n");
  Utils::formulaOutput(stdout, abcd);printf("\n");
  
  printf("after CNF\n");
  queue<_formula*> aux;
  abcd = CNFUtils::convertCNFWithAux(abcd, aux);
  Utils::formulaOutput(stdout, abcd); printf("\n");
  
  while(!aux.empty()) {
    _formula* faux = aux.front();
    aux.pop();
    CNFUtils::convertCNFWithAux(faux, aux);
    Utils::formulaOutput(stdout, faux);printf("\n");
  }
}

int main(int argc, char** argv) {
  int method = 1;
  fout = stdout;

  if(argc < 2) {
    yyin = fopen("res/input/example", "r");
    fout = fopen("res/output/sample.out", "w+");
  }
  else {
    int extra = 0;
    int files = 0;
    for(int i = 1; i < argc; i++) {
      if(extra < 1) {
        if(strcmp(argv[i], "-qbf") == 0) method = 0;
        if(strcmp(argv[i], "-sat") == 0) method = 1;
      }
      if(files < 2) {
        if(strncmp(argv[i], "-", 1) != 0) {
          if(files == 0) yyin = fopen(argv[i], "r");
          if(files == 1) fout = fopen(argv[i], "w+");
          files++;
        }
      }
    }
  }  
  
  yyparse();
  fclose(yyin);

//  for(int i = 0; i < G_NLP.size(); i++) {
//      G_NLP.at(i).output(stdout);
//  }
//  printf("\n");
  
//  testCNF();
  if(method == 0) {
    RepairQBF RQBF(G_NLP, Queries);
    vector<_formula*> fQBF;
    RQBF.computeRepairQBFFormula(fQBF);
    vector< set<int> > input = Utils::convertToSATInput(fQBF);
    
    int varNum = Vocabulary::instance().newAux(0) - 1;
    fprintf(fout, "p cnf %d %ld\n", varNum, input.size());
    vector<int> evarNum;
    for(int i = 1; i <= varNum; i++) evarNum.push_back(i);
    //Existential Predicate
    fprintf(fout, "e ");
    vector<int>& p = RQBF.choicePred;
    for(size_t i = 0; i < p.size(); i++) {
      fprintf(fout, "%d ", p[i]);
      evarNum[p[i] - 1] = 0;
    }
    fprintf(fout, "0\n");
    //Universal Predicate
    fprintf(fout, "a ");
    vector<int>& isp = RQBF.isomChoicePred;
    for(size_t i = 0; i < isp.size(); i++) {
      fprintf(fout, "%d ", isp[i]);
      evarNum[isp[i] - 1] = 0;
    }
    fprintf(fout, "0\n");
    //Existential Predicate
    fprintf(fout, "e ");
    for(size_t i = 0; i < evarNum.size(); i++) {
      if(evarNum[i] == 0) continue;
      fprintf(fout, "%d ", evarNum[i]);
    }
    fprintf(fout, "0\n");
    //SAT clause
    for(size_t i = 0; i< input.size(); i++) {
      for(set<int>::iterator it = input[i].begin(); it != input[i].end(); it++) {
        fprintf(fout, "%d ", *it);
      }
      fprintf(fout, "0\n");
    }
  }
  if(method == 1) {
    RepairSAT RSAT(G_NLP, Queries);
    vector<_formula*> fSAT;
    RSAT.computeRepairSATFormula(fSAT);
    
    vector< set<int> > input = Utils::convertToSATInput(fSAT);
    
    int varNum = Vocabulary::instance().newAux(0) - 1;
    fprintf(fout, "p cnf %d %ld\n", varNum, input.size());
    
    for(size_t i = 0; i< input.size(); i++) {
      for(set<int>::iterator it = input[i].begin(); it != input[i].end(); it++) {
        fprintf(fout, "%d ", *it);
      }
      fprintf(fout, "0\n");
    }
  }
  
  return 0;
}