/* 
 * File:   GLTranslator.cpp
 * Author: sharpen
 * 
 * Created on October 13, 2013, 10:28 PM
 */

#include <vector>

#include "GLTranslator.h"
#include "Rule.h"

GLTranslator::GLTranslator(vector<Rule> nlp, set<int> _Mset) :
        Mset(_Mset) {
    for(vector<Rule>::iterator it = nlp.begin(); it != nlp.end(); it++) {
        for(vector<int>::iterator nit = it->negative_literals.begin();
                nit != it->negative_literals.end();) {
            if(Mset.find(*nit) == Mset.end()) {
                nit = it->negative_literals.erase(nit);
            }
            else {
                nit++;
            }
        }
    }
    for(vector<Rule>::iterator it = nlp.begin(); it != nlp.end(); it++) {
        if(it->type == FACT) {
            facts.push_back(it->head);
        }
        else {
            if(it->negative_literals.size() == 0) {
               GL_nlp.push_back(*it);
            }   
        }
    }
}

GLTranslator::GLTranslator(const GLTranslator& orig) {
}

GLTranslator::~GLTranslator() {
}

void GLTranslator::calCons() {    
    for(vector<int>::iterator it = facts.begin(); it != facts.end(); it++) {
        for(vector<Rule>::iterator nit = GL_nlp.begin(); nit != GL_nlp.end(); nit++) {           
            for(vector<int>::iterator pit = nit->positive_literals.begin();
                    pit != nit->positive_literals.end(); pit++) {
                if(*it == *pit) {
                    nit->positive_literals.erase(pit);
                    break;
                }
            }
            if(nit->positive_literals.size() == 0) facts.push_back(nit->head);
        }
    }
}