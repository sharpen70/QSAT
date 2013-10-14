/* 
 * File:   GLTranslator.cpp
 * Author: sharpen
 * 
 * Created on October 13, 2013, 10:28 PM
 */

#include <vector>
#include "GLTranslator.h"
#include "Rule.h"
#include "Vocabulary.h"

GLTranslator::GLTranslator(vector<Rule> _nlp) :
        nlp(_nlp) {
}

GLTranslator::GLTranslator(const GLTranslator& orig) {
}

GLTranslator::~GLTranslator() {
}

//void GLTranslator::simplifyLP(vector<Rule>& GL_nlp, int fact) {
//    
//}
bool GLTranslator::isAnswerSet(set<int> Mset) {
    vector<Rule> _nlp = nlp;
    vector<Rule> GL_nlp;
    vector<int> facts;
    set<int> answers;
    
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
            if(it->type == RULE && it->negative_literals.size() == 0) {
               GL_nlp.push_back(*it);
            }   
        }
    }  
    for(int i = 0; i < facts.size(); i++) {
        answers.insert(facts.at(i));
        for(vector<Rule>::iterator nit = GL_nlp.begin(); nit != GL_nlp.end();) {
            if(facts.at(i) == nit->head) {
                nit = GL_nlp.erase(nit);
                continue;
            }
            for(vector<int>::iterator pit = nit->positive_literals.begin();
                    pit != nit->positive_literals.end(); pit++) {
                if(facts.at(i) == *pit) {
                    nit->positive_literals.erase(pit);
                    break;
                }
            }
            if(nit->positive_literals.size() == 0) {
                facts.push_back(nit->head);
                answers.insert(nit->head);
                nit = GL_nlp.erase(nit);
            }
            else {
                nit++;               
            }
        }
    }
    
    if(answers.size() != Mset.size()) return false;
//    for(set<int>::iterator it = Mset.begin(); it != Mset.end(); it++) {
//        printf("%s ", Vocabulary::instance().getAtom(*it));
//       // if(Mset.find(*it) == Mset.end()) return false;
//    }
//    printf("\n");
    for(set<int>::iterator it = answers.begin(); it != answers.end(); it++) {
        printf("%s ", Vocabulary::instance().getAtom(*it));
        if(Mset.find(*it) == Mset.end()) return false;
    }
    
    return true;
}