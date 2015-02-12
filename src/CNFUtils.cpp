/* 
 * File:   CNFUtils.cpp
 * Author: sharpen
 * 
 * Created on September 3, 2013, 9:21 PM
 */

#include "CNFUtils.h"
#include "Utils.h"
#include "NNFUtils.h"
#include <stack>
#include <assert.h>

vector<_formula*> CNFUtils::convertCNFSet(_formula* fml) {
  fml = NNFUtils::convertToNegativeNormalForm(fml);
  fml = convertToConjuntiveNormalForm(fml);
  
  vector<_formula*> result;
  divideCNFFormula(fml, result);
  
  return result;
}

_formula* CNFUtils::convertCNFWithAux(_formula* fml, queue<_formula*>& aux) {
 // Utils::formulaOutput(stdout, fml);printf("\n");
  fml = CNFUtils::convertToNegativeNormalForm(fml);
// Utils::formulaOutput(stdout, fml);printf("\n");
  fml = convertToConjuntiveNormalFormWithAux(fml, aux);
  return fml;
}

_formula* CNFUtils::convertToNegativeNormalForm(_formula* _originalFml) {
  _formula* nnf = NULL; 
  
  queue<Fin> fq;
  fq.push(Fin(_originalFml, NULL, true));
  
  while(!fq.empty()) {
    Fin fin = fq.front();
    _formula* f = fin.f;
    _formula* prec = fin.prec;
    bool left = fin.left;
    
    fq.pop();
    if(f->formula_type == ATOM) {
      if(nnf == NULL) nnf = f;
    }
    else if(f->formula_type == NEGA) {
      _formula* fl = f->subformula_l;
      
      if(fl->formula_type == NEGA) {
        if(prec == NULL) nnf = fl->subformula_l;
        else {
          if(left) prec->subformula_l = fl->subformula_l;
          else prec->subformula_r = fl->subformula_l;
        }
        fq.push(Fin(fl->subformula_l, prec, true));
        free(fl);
        free(f);
      }
      else if(fl->formula_type == CONJ) {
        _formula* nfl = Utils::compositeByConnective(NEGA, fl->subformula_l);
        _formula* nfr = Utils::compositeByConnective(NEGA, fl->subformula_r);
        _formula* cf = Utils::compositeByConnective(DISJ, nfl, nfr);
        
        if(prec == NULL) nnf = cf;
        else {
          if(left) prec->subformula_l = cf;
          else prec->subformula_r = cf;
        }
        fq.push(Fin(nfl, cf, true));
        fq.push(Fin(nfr, cf, false));
        free(fl);
        free(f);
      }
      else if(fl->formula_type == DISJ) {
        _formula* nfl = Utils::compositeByConnective(NEGA, fl->subformula_l);
        _formula* nfr = Utils::compositeByConnective(NEGA, fl->subformula_r);
        _formula* cf = Utils::compositeByConnective(CONJ, nfl, nfr);
        
        if(prec == NULL) nnf = cf;
        else {
          if(left) prec->subformula_l = cf;
          else prec->subformula_r = cf;
        }
        fq.push(Fin(nfl, cf, true));
        fq.push(Fin(nfr, cf, false));
        free(fl);
        free(f);        
      }
    }
    else {
      if(nnf == NULL) nnf = f;
      fq.push(Fin(f->subformula_l, f, true));
      fq.push(Fin(f->subformula_r, f, false));
    }
  }
  
  return nnf;
}

_formula* CNFUtils::convertWithCNFSubTree(_formula* fml, queue<_formula*>& auxRules) {
  queue<Fin> qf;
  qf.push(Fin(fml, NULL, true));
  
  while(!qf.empty()) {
    Fin fin = qf.front();
    qf.pop();
    
    _formula* f = fin.f;
    _formula* prec = fin.prec;
    bool left = fin.left;
    
    if(f->formula_type == DISJ) {      
      _formula* subfor_l = f->subformula_l;
      _formula* subfor_r = f->subformula_r;
      
      if(subfor_l->formula_type == CONJ || subfor_r->formula_type == CONJ) {
        if(subfor_l->formula_type == CONJ && subfor_r->formula_type == CONJ)
        {
          int auxA, auxB;
          _formula* f1 = computeAuxiliaryRule(subfor_l, auxA);
          _formula* f2 = computeAuxiliaryRule(subfor_r, auxB);
          auxRules.push(f1);
          auxRules.push(f2);
          
          f->subformula_l = Utils::compositeToAtom(auxA);
          f->subformula_r = Utils::compositeToAtom(auxB);
        }
        else {
          if(subfor_r->formula_type == CONJ) {
            f->subformula_l = subfor_r;
            f->subformula_r = subfor_l;
            subfor_l = f->subformula_l;
            subfor_r = f->subformula_r;
          }
          _formula* f1 = Utils::compositeByConnective(DISJ, subfor_l->subformula_l, subfor_r);
          _formula* f2 = Utils::compositeByConnective(DISJ, subfor_l->subformula_r, Utils::copyFormula(subfor_r));
          _formula* f12 = Utils::compositeByConnective(CONJ, f1, f2);
          
          if(prec == NULL) fml = f12;
          else {
            if(left) prec->subformula_l = f12;
            else prec->subformula_r = f12;
          }
          
          qf.push(Fin(f1, f12, true));
          qf.push(Fin(f2, f12, false));
        }
      }
    }
  }
  
  return fml;
}

_formula* CNFUtils::convertToConjuntiveNormalFormWithAux(_formula* fml, 
        queue<_formula*>& auxRules) {
  
  queue<Fin> qf;
  stack<Fin> sf;
  
  qf.push(Fin(fml, NULL, true));

  while(!qf.empty()) {
    Fin fin = qf.front();
    qf.pop();
    
    sf.push(fin);
    _formula* f = fin.f;
    
    if(f->formula_type == CONJ || f->formula_type == DISJ) {
      qf.push(Fin(f->subformula_l, f, true));
      qf.push(Fin(f->subformula_r, f, true));
    }
  }
  
  while(!sf.empty()) {
    Fin fin = sf.top();
    sf.pop();
    
    _formula* f = fin.f;
    _formula* prec = fin.prec;
    bool left = fin.left;
    
    _formula* st = convertWithCNFSubTree(f, auxRules);
  
    if(prec == NULL) fml = st;
    else {
      if(left) prec->subformula_l = st;
      else prec->subformula_r = st;
    }
  }
  
  return fml;
}

//_formula* CNFUtils::convertToConjuntiveNormalFormWithAux(_formula* fml, 
//        queue<_formula*>& auxRules) {
//  
//  queue<Fin> qf;
//  qf.push(Fin(fml, NULL, true));
//  
//  while(!qf.empty()) {
//    Fin fin = qf.front();
//    qf.pop();
//    
//    _formula* f = fin.f;
//    _formula* prec = fin.prec;
//    bool left = fin.left;
//    
//    if(f->formula_type == DISJ) {
//      f->subformula_l = convertToConjuntiveNormalFormWithAux(f->subformula_l, auxRules);
//      f->subformula_r = convertToConjuntiveNormalFormWithAux(f->subformula_r, auxRules);
//      
//      _formula* subfor_l = f->subformula_l;
//      _formula* subfor_r = f->subformula_r;
//      
//      if(subfor_l->formula_type == CONJ || subfor_r->formula_type == CONJ) {
//        if(subfor_l->formula_type == CONJ && subfor_r->formula_type == CONJ)
//        {
//          int auxA, auxB;
//          _formula* f1 = computeAuxiliaryRule(subfor_l, auxA);
//          _formula* f2 = computeAuxiliaryRule(subfor_r, auxB);
//          auxRules.push(f1);
//          auxRules.push(f2);
//          
//          f->subformula_l = Utils::compositeToAtom(auxA);
//          f->subformula_r = Utils::compositeToAtom(auxB);
//        }
//        else {
//          if(subfor_r->formula_type == CONJ) {
//            f->subformula_l = subfor_r;
//            f->subformula_r = subfor_l;
//            subfor_l = f->subformula_l;
//            subfor_r = f->subformula_r;
//          }
//          _formula* f1 = Utils::compositeByConnective(DISJ, subfor_l->subformula_l, subfor_r);
//          _formula* f2 = Utils::compositeByConnective(DISJ, subfor_l->subformula_r, Utils::copyFormula(subfor_r));
//          _formula* f12 = Utils::compositeByConnective(CONJ, f1, f2);
//          
//          if(prec == NULL) fml = f12;
//          else {
//            if(left) prec->subformula_l = f12;
//            else prec->subformula_r = f12;
//          }
//          
//          qf.push(Fin(f1, f12, true));
//          qf.push(Fin(f2, f12, false));
//        }
//      }
//    }
//    else if(f->formula_type == CONJ) {
//      qf.push(Fin(f->subformula_l, f, true));
//      qf.push(Fin(f->subformula_r, f, false));
//    }
//    else if(f->formula_type == NEGA) {
//      qf.push(Fin(f->subformula_l, f, true));
//    }
//    else if(f->formula_type == ATOM) {
//      
//    }   
//  }
//  
//  return fml;
//}

//_formula* CNFUtils::convertToConjuntiveNormalFormWithAux(_formula* fml,
//        queue<_formula*>& auxRules) {
//  if(fml->formula_type == DISJ) {
//    fml->subformula_l = convertToConjuntiveNormalFormWithAux(fml->subformula_l, auxRules);
//    fml->subformula_r = convertToConjuntiveNormalFormWithAux(fml->subformula_r, auxRules);
//    
//    _formula* subfor_l = fml->subformula_l;
//    _formula* subfor_r = fml->subformula_r;
//    
//    if(subfor_l->formula_type == CONJ || subfor_r->formula_type == CONJ) {
//      if(subfor_l->formula_type == CONJ && subfor_r->formula_type == CONJ)
//      {
//        int auxA, auxB;
//        _formula* f1 = computeAuxiliaryRule(subfor_l, auxA);
//        _formula* f2 = computeAuxiliaryRule(subfor_r, auxB);
//        auxRules.push(f1);
//        auxRules.push(f2);
//
//        fml->subformula_l = Utils::compositeToAtom(auxA);
//        fml->subformula_r = Utils::compositeToAtom(auxB);
//      }
//      else {
//        if(subfor_r->formula_type == CONJ) {
//          fml->subformula_l = subfor_r;
//          fml->subformula_r = subfor_l;
//          subfor_l = fml->subformula_l;
//          subfor_r = fml->subformula_r;
//        }
//        _formula* f1 = Utils::compositeByConnective(DISJ, subfor_l->subformula_l, subfor_r);
//        _formula* f2 = Utils::compositeByConnective(DISJ, subfor_l->subformula_r, Utils::copyFormula(subfor_r));
//        
//        fml = Utils::compositeByConnective(CONJ, f1, f2);
//        fml->subformula_l = convertToConjuntiveNormalFormWithAux(fml->subformula_l, auxRules);
//        fml->subformula_r = convertToConjuntiveNormalFormWithAux(fml->subformula_r, auxRules);
//      }
//    }
//  }
//  else if(fml->formula_type == CONJ || fml->formula_type == IMPL) {
//    fml->subformula_l = convertToConjuntiveNormalFormWithAux(fml->subformula_l, auxRules);
//    fml->subformula_r = convertToConjuntiveNormalFormWithAux(fml->subformula_r, auxRules);
//  }
//  else if(fml->formula_type == NEGA) {
//    fml->subformula_l = convertToConjuntiveNormalFormWithAux(fml->subformula_l, auxRules);
//  }
//  else if(fml->formula_type == ATOM) {
//    
//  }
//  
//  return fml;
//}

_formula* CNFUtils::computeAuxiliaryRule(_formula* fml, int& aux) {
  aux = Vocabulary::instance().newAux(1);
  
  _formula* impl_l = Utils::compositeByConnective(DISJ, 
          Utils::compositeByConnective(NEGA, Utils::compositeToAtom(aux)), fml);
  _formula* impl_r = Utils::compositeByConnective(DISJ,
          Utils::compositeToAtom(aux), Utils::compositeByConnective(NEGA, 
          Utils::copyFormula(fml)));
  
  _formula* res = Utils::compositeByConnective(CONJ, impl_l, impl_r);

  return res;
}

_formula* CNFUtils::convertToConjuntiveNormalForm(_formula* fml) {
  if(fml->formula_type == DISJ) {
    fml->subformula_l = convertToConjuntiveNormalForm(fml->subformula_l);
    fml->subformula_r = convertToConjuntiveNormalForm(fml->subformula_r);
    
    _formula* subfor_l = fml->subformula_l;
    _formula* subfor_r = fml->subformula_r;
    
    if(subfor_l->formula_type == CONJ || subfor_r->formula_type == CONJ) {
      if(subfor_l->formula_type == CONJ && subfor_r->formula_type == CONJ)
      {
        _formula* f1 = Utils::compositeByConnective(DISJ, subfor_l->subformula_l, 
                subfor_r->subformula_l);
        _formula* f2 = Utils::compositeByConnective(DISJ, subfor_l->subformula_r,
                Utils::copyFormula(subfor_r->subformula_l));
        _formula* f3 = Utils::compositeByConnective(DISJ, Utils::copyFormula(subfor_l->subformula_l),
                subfor_r->subformula_r);
        _formula* f4 = Utils::compositeByConnective(DISJ, Utils::copyFormula(subfor_l->subformula_r),
                Utils::copyFormula(subfor_r->subformula_r));
        
        _formula* f12 = Utils::compositeByConnective(CONJ, f1, f2);
        _formula* f34 = Utils::compositeByConnective(CONJ, f3, f4);
        
        fml = Utils::compositeByConnective(CONJ, f12, f34);
      }
      else {
        if(subfor_r->formula_type == CONJ) {
          fml->subformula_l = subfor_r;
          fml->subformula_r = subfor_l;
          subfor_l = fml->subformula_l;
          subfor_r = fml->subformula_r;
        }
        _formula* f1 = Utils::compositeByConnective(DISJ, subfor_l->subformula_l, subfor_r);
        _formula* f2 = Utils::compositeByConnective(DISJ, subfor_l->subformula_r, Utils::copyFormula(subfor_r));
        
        fml = Utils::compositeByConnective(CONJ, f1, f2);
      }
      fml->subformula_l = convertToConjuntiveNormalForm(fml->subformula_l);
      fml->subformula_r = convertToConjuntiveNormalForm(fml->subformula_r);
    }
  }
  else if(fml->formula_type == CONJ || fml->formula_type == IMPL) {
    fml->subformula_l = convertToConjuntiveNormalForm(fml->subformula_l);
    fml->subformula_r = convertToConjuntiveNormalForm(fml->subformula_r);
  }
  else if(fml->formula_type == NEGA) {
    fml->subformula_l = convertToConjuntiveNormalForm(fml->subformula_l);
  }
  else if(fml->formula_type == ATOM) {
    
  }
  
  return fml;
}

void CNFUtils::divideCNFFormula(_formula* fml, vector<_formula*>& division) {
  queue<_formula*> fq;
  fq.push(fml);
  
  while(!fq.empty()) {
    _formula* f = fq.front();
    fq.pop();
    
    if(f->formula_type == CONJ) {
      fq.push(f->subformula_l);
      fq.push(f->subformula_r);
    }
    else {
      division.push_back(f);
    }
  }
}

//void CNFUtils::divideCNFFormula(_formula* fml, vector<_formula*>& division) {
//  if(fml->formula_type == CONJ) {
//    divideCNFFormula(fml->subformula_l, division);
//    divideCNFFormula(fml->subformula_r, division);
//  }
//  else {
//    division.push_back(fml);
//  }
//}

