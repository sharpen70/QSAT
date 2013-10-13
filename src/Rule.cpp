#include "Rule.h"
#include "structs.h"
#include "Vocabulary.h"
#include <assert.h>
#include <cstdlib>

Rule::Rule(_rule* r) : 
        head(r->head), body_length(r->length){
    printf("length %d", body_length);
    for(int i = 0; i < (r->length); i++) {
        if(r->body[i] > 0) {
            printf("p l\n");
            this->positive_literals.push_back(r->body[i]);
        }
        else {
            printf("n l\n");
            this->negative_literals.push_back(-1 * r->body[i]);
        }
    }
}
Rule::Rule(const Rule& _rhs) : 
        head(_rhs.head),
        body_length(_rhs.body_length),
        positive_literals(_rhs.positive_literals),
        negative_literals(_rhs.negative_literals) {
}
Rule::~Rule() {
    positive_literals.clear();
    negative_literals.clear();
}
Rule& Rule::operator = (const Rule& _rhs) {
    head = _rhs.head;
    positive_literals = _rhs.positive_literals;
    negative_literals = _rhs.negative_literals;
    return *this;
}

void Rule::output(FILE* _out) const {
    if(head > 0)
        fprintf(_out, "%s", Vocabulary::instance().getAtom(head));
    
    if(body_length > 0) {
        fprintf(_out, " :- ");
        for(int i = 0; i < positive_literals.size(); i++) {
            fprintf(_out, "%s", Vocabulary::instance().getAtom(positive_literals.at(i)));
            if(i != positive_literals.size() - 1) {
                fprintf(_out, ",");
            }
        }
        for(int i = 0; i < negative_literals.size(); i++) {
            if(positive_literals.size() != 0) {
                fprintf(_out, ",");
            }
            fprintf(_out, "not %s", Vocabulary::instance().getAtom(negative_literals.at(i)));            
        }
    }
    
    fprintf(_out, "\n");
}