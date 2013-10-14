/* 
 * File:   GLTranslator.h
 * Author: sharpen
 *
 * Created on October 13, 2013, 10:28 PM
 */

#ifndef GLTRANSLATOR_H
#define	GLTRANSLATOR_H

#include <set>
#include "Rule.h"
#include <vector>
using namespace std;

class GLTranslator {
public:
    GLTranslator(vector<Rule> nlp, set<int> _Mset);
    GLTranslator(const GLTranslator& orig);
    ~GLTranslator();
    bool isAnswerSet();
private:
    void calCons();
    
    vector<Rule> GL_nlp;
    vector<int> facts;
    set<int> Mset;
    set<int> Cons;
};

#endif	/* GLTRANSLATOR_H */

