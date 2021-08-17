/*
 * Write a lex program to replace the sequence of white spaces by a single blank from input.
 */

/* Declarations */

%{
#include<stdio.h>
%}

/* Rules  */
%%

([ ]+) {
    printf(" ");
}

. {
    printf("%s",yytext);
}

%%

/* User Routines */

int yywrap(){}

int main()
{
	yylex();
	return 0;
}
