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
    static _formula* copyFormula(const _formula* _fml);
    static void deleteFormula(_formula* _fml);
    
    static void formulaOutput(FILE* out, const _formula* fml);
    
    static vector<_formula*> joinFormulas(vector<_formula*> des_list, vector<_formula*> join_list);
    
    static bool inList(int tag, set<int> list);
    static vector<int> divideListAt(int tag, vector<int> list);
    static bool crossList(vector<int> l1, set<int> l2);
    
    static vector< set<int> > convertToSATInput(vector<_formula*> cnfDlp);
    static void convertCNFformulaToLits(_formula* rule, set<int>& lits);
    
    static _formula* convertRuleBodyToFormula(const Rule& rule);
    
    static vector< vector< vector<char*> > > readClaspAnswers(const char* AnswerSet_list);
    static vector< vector<char*> > readClaspAnswer(const char* answer);
    
    static bool charCmp(char* a, char* b);
    static bool ModelCharCmp(vector<char*>& a, vector<char*>& b);
    
    static bool compareSingleModel(vector<char*>& claspModel, set<int>& satModel);
    static bool compareAnswerSet(vector< vector<char*> >& claspAnswer, vector< set<int> >& satAnswer);
    
};



#endif	/* UTIL_H */

