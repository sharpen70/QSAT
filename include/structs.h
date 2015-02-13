#ifndef STRUCTS_H
#define	STRUCTS_H

#define MAX_ATOM_LENGTH 16

#include <map>
#include <set>
#include <vector>
// Type definition
////////////////////////////////////////////////////////////////////////////////

enum BOOL {
  FALSE = 0,
  TRUE = 1
};

enum SYMBOL_TYPE {
  VARIABLE = 0,
  FUNCTION,
  PREDICATE,
  DOMAIN
};

enum RULE_TYPE {
  FACT = 0,
  CONSTRANT,
  RULE,
  QUERY
};

enum ATOM_TYPE {
  POSITIVE = 0,
  NEGATIVE,
  SELECT,
  Q
};

enum FORMULA_TYPE {
  ATOM = 0,	//atom
  NEGA,		//negation
  CONJ,		//conjunction
  DISJ,		//disjunction
  IMPL,		//implication
  UNIV,		//universal
  EXIS		//existential
};

// Structures
typedef struct __atom {
  ATOM_TYPE type;
  int index;
}_atom;

typedef struct __rule {
  _atom* head;
  _atom* body[MAX_ATOM_LENGTH];
  int length;
  RULE_TYPE type;
}_rule;

typedef struct __literals {
  _atom* atoms[MAX_ATOM_LENGTH];
  int length;    
}_literals;

typedef struct __formula {
  FORMULA_TYPE formula_type;

  union {
    __formula* subformula_l;   //NEGA,CONJ,DISJ,IMPL,UNIV,EXIS
    int predicate_id;                 //ATOM
  };
  
  union {
    __formula* subformula_r;   //CONJ,DISJ,IMPL
  };
}_formula;

struct Fin {
  _formula* f;
  _formula* prec;
  bool left;
  
  Fin(_formula* ff, _formula* p, bool l):f(ff),prec(p),left(l){}
};
#endif



