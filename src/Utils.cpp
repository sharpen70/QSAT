#include "Utils.h"
#include "Rule.h"
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

/**
 * 使用联接词(非、析取、合取、蕴含)生成公式
 * @param _formulaType 联接词
 * @param _subformulaL 左公式
 * @param _subformulaR 右公式
 * @return 新公式
 */
_formula* Utils::compositeByConnective(FORMULA_TYPE _formulaType, 
        _formula* _subformulaL, _formula* _subformulaR) {
  assert(_formulaType == NEGA || _formulaType == DISJ ||
          _formulaType == CONJ || _formulaType == IMPL);
  
  _formula* fml = (_formula*)malloc(sizeof(_formula));
  assert(fml);
  
  fml->formula_type = _formulaType;
  fml->subformula_l = _subformulaL;
  fml->subformula_r = _subformulaR;
  
  return fml;
}
/**
 * 生成atom
 * @param _formulaType
 * @param _predicateId
 * @param _parameters
 * @return 
 */
_formula* Utils::compositeToAtom(int _atom_id) {
  _formula* fml = (_formula*)malloc(sizeof(_formula));
  assert(fml);
  
  fml->formula_type = ATOM;
  fml->predicate_id = _atom_id;
  
  return fml;
}

_formula* Utils::copyFormula(_formula* _fml) {
  if(_fml == NULL) return NULL;

  _formula* fml = NULL;

  queue<Fin> fq;
  fq.push(Fin(_fml, NULL, true));
  
  while(!fq.empty()) {
    Fin fin = fq.front();
    _formula* f = fin.f;
    _formula* prec = fin.prec;
    bool left = fin.left;
    fq.pop();
    
    _formula* newFormula = (_formula*)malloc(sizeof(_formula));
    memcpy(newFormula, f, sizeof(_formula));
    
    if(prec == NULL) fml = newFormula;
    else {
      if(left) prec->subformula_l = newFormula;
      else prec->subformula_r = newFormula;
    }
    
    switch(f->formula_type) {
      case ATOM:
        newFormula->predicate_id = f->predicate_id;
        break;
      case CONJ:
      case DISJ:
        assert(f->subformula_r);
        fq.push(Fin(f->subformula_r, newFormula, false));
      case NEGA:
        assert(f->subformula_l);
        fq.push(Fin(f->subformula_l, newFormula, true));
        break;
      default:
        assert(0);
    }
  }
  
  return fml;
}

//_formula* Utils::copyFormula(const _formula* _fml) {
//  if (_fml == NULL) {
//    return NULL;
//  }
//	
//  _formula* newFormula = (_formula*)malloc(sizeof(_formula));
//  assert (newFormula);
//  
//  memcpy(newFormula, _fml, sizeof(_formula));
//  switch (_fml->formula_type)
//  {
//    case ATOM:
//      newFormula->predicate_id = _fml->predicate_id;
//      break;
//    case CONJ:
//    case DISJ:
//    case IMPL:
//      assert(_fml->subformula_r);
//      newFormula->subformula_r = copyFormula( _fml->subformula_r);
//    case NEGA:
//    case UNIV:
//    case EXIS:
//      assert(_fml->subformula_l);
//      newFormula->subformula_l = copyFormula(_fml->subformula_l);
//      break;
//    default:
//      assert (0);
//  }
//  
//  return newFormula;
//}

_formula* Utils::copyIsomorFormula(const _formula* _fml, int n) {
  if (_fml == NULL) {
    return NULL;
  }
	
  _formula* newFormula = (_formula*)malloc(sizeof(_formula));
  assert (newFormula);
  
  memcpy(newFormula, _fml, sizeof(_formula));
  switch (_fml->formula_type)
  {
    case ATOM:
      newFormula->predicate_id = _fml->predicate_id + n * (Vocabulary::instance().apSize());
      break;
    case CONJ:
    case DISJ:
    case IMPL:
      assert(_fml->subformula_r);
      newFormula->subformula_r = copyIsomorFormula(_fml->subformula_r, n);
    case NEGA:
    case UNIV:
    case EXIS:
      assert(_fml->subformula_l);
      newFormula->subformula_l = copyIsomorFormula(_fml->subformula_l, n);
      break;
    default:
      assert (0);
  }
  
  return newFormula;  
}

void Utils::deleteFormula(_formula* _fml) {
  assert(_fml);
  
  queue<_formula*> qf;
  qf.push(_fml);
  
  while(!qf.empty()) {
    _formula* f = qf.front();
    qf.pop();
    switch(f->formula_type) {
      case ATOM: break;
      case DISJ:
      case CONJ:
        qf.push(f->subformula_r);
      case NEGA:
        qf.push(f->subformula_l);
        break;
      default:
        assert(0);
    }
            
    free(f);
  }
}

//void Utils::deleteFormula(_formula* _fml) {
//  assert(_fml);
//  
//  switch (_fml->formula_type)
//  {
//    case ATOM:
//      break;
//    case CONJ:
//    case DISJ:
//    case IMPL:
//      assert(_fml->subformula_r);
//      deleteFormula(_fml->subformula_r);
//    case NEGA:
//    case UNIV:
//    case EXIS:
//      assert(_fml->subformula_l);
//      deleteFormula(_fml->subformula_l);
//      break;
//    default:
//      assert ( 0 );
//  }
//  
//  free(_fml);
//}

vector< set<int> > Utils::convertToSATInput(vector<_formula*> cnfNlp) {
  vector< set<int> > res;
  for(int i = 0; i < cnfNlp.size(); i++) {
    set<int> lits;
    convertCNFformulaToLits(cnfNlp[i], lits);    
    res.push_back(lits);   
  }
  return res;
}

void Utils::convertCNFformulaToLits(_formula* rule, set<int>& lits) {
  queue<_formula*> qf;
  qf.push(rule);
  
  while(!qf.empty()) {
    _formula* f = qf.front();
    qf.pop();
    
    if(f->formula_type == ATOM) lits.insert(f->predicate_id);
    else if(f->formula_type == NEGA) lits.insert(-1 * (f->subformula_l->predicate_id));
    else {
      qf.push(f->subformula_l);
      qf.push(f->subformula_r);
    }
  }
}

//void Utils::convertCNFformulaToLits(_formula* rule, set<int>& lits) {
//  if(rule->formula_type == ATOM) {
//    lits.insert(rule->predicate_id);
//  }
//  else if(rule->formula_type == NEGA) {
//    lits.insert(-1 * (rule->subformula_l->predicate_id));
//  }
//  else {
//    convertCNFformulaToLits(rule->subformula_l, lits);
//    convertCNFformulaToLits(rule->subformula_r, lits);
//  }
//}

_formula* Utils::convertRuleBodyToFormula(const Rule& rule) {
  _formula* fml = NULL;
  
  Rule _rule(rule);
  
  if(rule.type == FACT) return NULL;
  
  for(set<int>::iterator it = _rule.positive_literals.begin(); it != 
          _rule.positive_literals.end(); it++) {
    if(fml == NULL) fml = Utils::compositeToAtom(*it);
    else {
      fml = Utils::compositeByConnective(CONJ, fml, Utils::compositeToAtom(*it));
    }
  }
  for(set<int>::iterator it = _rule.negative_literals.begin(); it !=
          _rule.negative_literals.end(); it++) {
    _formula* nega = Utils::compositeByConnective(NEGA, Utils::compositeToAtom(*it));
    if(fml == NULL) fml = nega;
    else {
      fml = Utils::compositeByConnective(CONJ, fml, nega);
    }
  }
  
  return fml;
}

void Utils::formulaOutput(FILE* out, _formula* fml) {
  assert(fml);
  
  _formula* output = copyFormula(fml);
  
  switch(output->formula_type) {
    case ATOM:
      if(output->predicate_id > Vocabulary::instance().apSize())
        fprintf(out, "aux_%d", output->predicate_id - Vocabulary::instance().apSize());
      else
        fprintf(out, "%s", Vocabulary::instance().getMapAtom(output->predicate_id));
      break;
    case CONJ:
      assert(output->subformula_l);
      fprintf(out, "(");
      formulaOutput(out, output->subformula_l);
      fprintf(out, " & ");
      assert(output->subformula_r);
      formulaOutput(out, output->subformula_r);
      fprintf(out, ")");
      break;
    case DISJ:
      assert(output->subformula_l);
      fprintf(out, "(");
      formulaOutput(out, output->subformula_l);
      fprintf(out, " | ");
      assert(output->subformula_r);
      formulaOutput(out, output->subformula_r);
      fprintf(out, ")");
      break;
    case IMPL:
      assert(output->subformula_l);
      fprintf(out, "(");
      formulaOutput(out, output->subformula_l);
      fprintf(out, " -> ");
      assert(output->subformula_r);
      formulaOutput(out, output->subformula_r);
      fprintf(out, ")");
      break;
    case NEGA:   
      fprintf(out, " not (");
      assert(output->subformula_l);
      formulaOutput(out, output->subformula_l);
      fprintf(out, ")");
      break;
    case UNIV:
    case EXIS:
      assert(output->subformula_l);
      formulaOutput(out, output->subformula_l);
      break;
    default:
      assert(0);
  }
  
  free(output);
}