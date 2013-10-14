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
    GLTranslator(vector<Rule> nlp);
    GLTranslator(const GLTranslator& orig);
    ~GLTranslator();
    bool isAnswerSet(set<int> Mset);
    void printfCons();
private:
    vector<Rule> nlp;
   // void simplifyLP(vector<Rule>& GL_nlp, int fact, set<int> answer);
};

#endif	/* GLTRANSLATOR_H */

