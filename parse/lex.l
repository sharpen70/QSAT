%{
#include "parse.h"

extern "C" {
	int yywrap(void);
	int yylex(void);
}
extern int context_flag;/* 0 - not in any atom, 1 - in an atom*/

%}

blank	  			[ \n\t\r]*
atom                            [a-zA-Z_0-9(),]+

%%
{atom}                                  {
                                            yylval.s=strdup(yytext); 
                                            if(strcmp(yytext, "not") == 0)
                                                return S_NEGA;

                                            return S_ATOM;
                                        }
":-"                                    {return S_IMPL;			}
";"                                     {return COLON;                  }
"."                                     {return PERIOD;                 }
{blank}		    	{/* skip blankspace */	}
%%

int yywrap()
{
  return 1;
}
