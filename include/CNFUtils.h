/* 
 * File:   CNFUtils.h
 * Author: sharpen
 *
 * Created on September 3, 2013, 9:21 PM
 */

#ifndef CNFUTILS_H
#define	CNFUTILS_H

#include <queue>
#include <vector>
#include "structs.h"

using namespace std;

class CNFUtils {
public:
  static vector<_formula*> convertCNFSet(_formula* fml);
  static _formula* convertCNFWithAux(_formula* fml, queue<_formula*>& aux);
  static _formula* convertToConjuntiveNormalForm(_formula* fml);
  static _formula* convertToConjuntiveNormalFormWithAux(_formula* fml, queue<_formula*>&);
  static _formula* convertWithCNFSubTree(_formula*, queue<_formula*>&);
  static _formula* computeAuxiliaryRule(_formula*, int&);
  static void divideCNFFormula(_formula* fml, vector<_formula*>& division);
  static _formula* convertToNegativeNormalForm(_formula* fml);
};

#endif	/* CNFUTILS_H */

