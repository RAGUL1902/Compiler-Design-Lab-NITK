/*
 * Write a lex program to print the copy of input. 
 */

/* Declarations */

%{
#include<stdio.h>
%}

/* Rules  */
%%

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
