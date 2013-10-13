/* 
 * File:   CNFUtils.h
 * Author: sharpen
 *
 * Created on September 3, 2013, 9:21 PM
 */

#ifndef CNFUTILS_H
#define	CNFUTILS_H

#include <vector>
#include "structs.h"

using namespace std;

class CNFUtils {
public:
    static vector<_formula*> convertCNF(_formula*& fml);
    static _formula* convertToConjuntiveNormalForm(_formula*& fml);
    static void divideCNFFormula(_formula* fml, vector<_formula*>& division);
    static _formula* convertToNegativeNormalForm(_formula*& fml);
};

#endif	/* CNFUTILS_H */

