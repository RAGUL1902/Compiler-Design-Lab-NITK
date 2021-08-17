/*
 * Write a lex program to replace the sequence of white spaces by a single blank from input 
 * text file and add the contents in output text file.
 */

/* Declarations */

%{
#include<stdio.h>
%}

/* Rules  */
%%

([ ]+) {
    fprintf(yyout," ");
}

. {
    fprintf(yyout,"%s",yytext);
}

%%

/* User Routines */

int yywrap(){}

int main()
{
    yyin = fopen("input.txt","r");
    yyout = fopen("output.txt","w");
	yylex();
	return 0;
}
