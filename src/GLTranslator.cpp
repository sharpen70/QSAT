/* 
 * File:   GLTranslator.cpp
 * Author: sharpen
 * 
 * Created on October 13, 2013, 10:28 PM
 */

#include <vector>
#include "GLTranslator.h"
#include "Rule.h"

GLTranslator::GLTranslator(vector<Rule> _nlp) :
        nlp(_nlp) {
}

GLTranslator::GLTranslator(const GLTranslator& orig) {
}

GLTranslator::~GLTranslator() {
}

bool GLTranslator::isAnswerSet(set<int> Mset) {
    vector<Rule> _nlp = nlp;
    vector<Rule> GL_nlp;
    vector<int> facts;
    
    //GL_transformation
    for(vector<Rule>::iterator it = _nlp.begin(); it != _nlp.end(); it++) {
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
    
    for(vector<Rule>::iterator it = _nlp.begin(); it != _nlp.end(); it++) {
        if(it->type == FACT) {
            facts.push_back(it->head);
        }
        else {
            if(it->negative_literals.size() == 0) {
               GL_nlp.push_back(*it);
            }   
        }
    }
    
    //Calculate cons
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
    
    if(facts.size() != Mset.size()) return false;
    
    for(vector<int>::iterator it = facts.begin(); it != facts.end(); it++) {
        if(Mset.find(*it) == Mset.end()) return false;
    }
    
    return true;
}