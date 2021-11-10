%{
#include"y.tab.h"
extern char yyval;
%}

%%
[0-9]+ {yylval.symbol = (char)yytext[0];return NUMBER;}
[a-zA-Z]+ {yylval.symbol =(char) yytext[0];return ID;}
. {return yytext[0];}
\n {return 0;}
%%
