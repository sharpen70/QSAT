/* 
 * File:   Util.h
 * Author: yzf
 *
 * Created on July 27, 2013, 1:33 AM
 */

#ifndef UTIL_H
#define	UTIL_H

#include <cstdio>
#include "structs.h"
#include <map>
#include <set>
#include <vector>
#include <string>
#include "Vocabulary.h"
#include "Rule.h"

using namespace std;

/**
 * 工具类
 */
class Utils {
public:    
    static _formula* compositeByConnective(FORMULA_TYPE _formulaType, 
                         _formula* _subformulaL, _formula* _subformulaR = NULL);
    static _formula* compositeToAtom(int _atom_id);
    static _formula* copyFormula(_formula* _fml);
    static _formula* copyIsomorFormula(const _formula*, int);
    static void deleteFormula(_formula* _fml);
    
    static void formulaOutput(FILE* out, _formula* fml);
    
    static vector< set<int> > convertToSATInput(vector<_formula*> cnfDlp);
    static void convertCNFformulaToLits(_formula* rule, set<int>& lits);
    
    static _formula* convertRuleBodyToFormula(const Rule& rule);
};



#endif	/* UTIL_H */

