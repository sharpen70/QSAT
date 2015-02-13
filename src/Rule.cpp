#include "Rule.h"
#include "structs.h"
#include "Vocabulary.h"
#include <assert.h>
#include <cstdlib>
#include <string>


void free_s_rule(_rule* r) {
  for(int i = 0; i < r->length; i++) free(r->body[i]);
  free(r->head);
  free(r);
}

Rule::Rule():type(RULE),head(0){}

Rule::Rule(_rule* r) {
  this->type = r->type;
  
  if(r->type != CONSTRANT) {
    if(r->head->type == Q) {
      this->type = QUERY;
      this->head = -1;
    }
    else this->head = r->head->index;
  }
  else
    this->head = 0;  

  this->sel_pred = -1;

  for(int i = 0; i < (r->length); i++) {
    if(r->body[i]->type == POSITIVE) {
      this->positive_literals.insert(r->body[i]->index);
    }
    else if(r->body[i]->type == SELECT) {
      this->sel_pred = r->body[i]->index;
      this->positive_literals.insert(r->body[i]->index);
    }
    else {
      this->negative_literals.insert(r->body[i]->index);
    }
  }
  
  free(r);
}

Rule::Rule(const Rule& _rhs) : 
head(_rhs.head),
        type(_rhs.type),
        positive_literals(_rhs.positive_literals),
        negative_literals(_rhs.negative_literals), sel_pred(_rhs.sel_pred) {
}

Rule::~Rule() {
  positive_literals.clear();
  negative_literals.clear();
}

Rule& Rule::operator = (const Rule& _rhs) {
  head = _rhs.head;
  positive_literals = _rhs.positive_literals;
  negative_literals = _rhs.negative_literals;
  type = _rhs.type;
  sel_pred = _rhs.sel_pred;

  return *this;
}

void Rule::output(FILE* _out) const {
  if(head > 0)
    fprintf(_out, "%d", head);
  
  if(type != FACT) {
    fprintf(_out, " :- ");
    for(set<int>::iterator pit = positive_literals.begin(); pit != 
            positive_literals.end(); pit++) {
      fprintf(_out, "%d", *pit);
      if(pit != (--positive_literals.end())) {
        fprintf(_out, ",");
      }
    }
    for(set<int>::iterator nit = negative_literals.begin(); nit !=
            negative_literals.end(); nit++) {
      if(positive_literals.size() != 0) {
        fprintf(_out, ",");
      }
      fprintf(_out, "not %d", *nit);            
    }
  }
  
  fprintf(_out, "\n");  
}
//void Rule::output(FILE* _out) const {
//  if(head > 0)
//    fprintf(_out, "%s", Vocabulary::instance().getMapAtom(head));
//  
//  if(type != FACT) {
//    fprintf(_out, " :- ");
//    for(set<int>::iterator pit = positive_literals.begin(); pit != 
//            positive_literals.end(); pit++) {
//      fprintf(_out, "%s", Vocabulary::instance().getMapAtom(*pit));
//      if(pit != (--positive_literals.end())) {
//        fprintf(_out, ",");
//      }
//    }
//    for(set<int>::iterator nit = negative_literals.begin(); nit !=
//            negative_literals.end(); nit++) {
//      if(positive_literals.size() != 0) {
//        fprintf(_out, ",");
//      }
//      fprintf(_out, "not %s", Vocabulary::instance().getAtom(*nit));            
//    }
//  }
//  
//  fprintf(_out, "\n");
//}