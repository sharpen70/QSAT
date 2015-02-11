%{
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include "Vocabulary.h"
#include "Rule.h"

extern "C" {
	void yyerror(const char *s);
	extern int yylex(void);
}
 
extern vector<Rule> G_NLP;
extern vector<Rule> Queries;
int id;

void yyerror(const char* s)
{
	printf("Parser error: %s\n", s);
}

%}

%union {
    char* s;
    int i;
    struct __literals* l;
    struct __rule* r;
    struct __atom* a;
}

%token <s> S_ATOM
%token <s> S_VARI
%token <s> S_NEGA
%token <s> S_IMPL
%token <s> LPAREN
%token <s> RPAREN
%token <s> PERIOD
%token <s> COLON

%type <a> literal
%type <l> literals
%type <r> rule

%left S_IMPL

%%
nlp 
    : rules {
    }
    |
;

rules
    : rules rule {
        Rule rule($2);
        if(rule.type == QUERY) Queries.push_back(rule);
        else G_NLP.push_back(rule);
    }
    | rule {
        Rule rule($1);
        if(rule.type == QUERY) Queries.push_back(rule);
        else G_NLP.push_back(rule);
    }
;

rule 
    : literal PERIOD{
        $$ = (_rule*)malloc(sizeof(_rule));
        $$->head = $1;
        $$->length = 0;
        $$->type = FACT;
    }
    | literal S_IMPL literals PERIOD{
        $$ = (_rule*)malloc(sizeof(_rule));
        $$->head = $1;
        $$->type = RULE;
        
        for(int i = 0; i < ($3->length); i++) {
            $$->body[i] = $3->atoms[i];
        }
        $$->length = $3->length;
    }
    | S_IMPL literals PERIOD{
        $$ = (_rule*)malloc(sizeof(_rule));
        $$->type = CONSTRANT;
        for(int i = 0; i < ($2->length); i++) {
            $$->body[i] = $2->atoms[i];
        }
        $$->length = $2->length;
    }
;

literals
    : literals COLON literal {
        $1->atoms[$1->length] = $3;
        $1->length++;
    }
    | literal {
        $$ = (__literals*)malloc(sizeof(_literals));
        $$->atoms[0] = $1;
        $$->length = 1;
    }
;

literal
    : S_NEGA S_ATOM {
        $$ = (_atom*)malloc(sizeof(_atom));
        $$->type = NEGATIVE;
        $$->index = Vocabulary::instance().addMapAtom($2);
    }
    | S_ATOM {
        $$ = (_atom*)malloc(sizeof(_atom));

        if(strcmp($1, "q") == 0) $$->type = Q;
        else if(strncmp($1, "sel", 3) == 0) {
            $$->type = SELECT;
            $$->index = Vocabulary::instance().addMapSelAtom($1);
        }
        else {
            $$->type = POSITIVE;
            $$->index = Vocabulary::instance().addMapAtom($1);
        }
    }
;
%%
