/*
 * Write a lex program to display the number of lines and number of characters in input.
 */

/* Declarations */

%{
#include<stdio.h>
int newLines = 1;
int chars = 0;
%}

/* Rules  */
%%

"\n" {
    newLines++;
}

. chars++;

%%

/* User Routines */

int yywrap(){}

int main()
{

	yylex();
	printf("Number of characters : %d\n", chars);
    printf("Number of lines : %d\n",newLines);
	return 0;
}
