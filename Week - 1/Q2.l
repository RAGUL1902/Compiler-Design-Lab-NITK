/*
 * Write a program for lexical analyzer to recognize all the valid tokens in the input 
 * program written according to the grammar.
 */

/* Declarations */

%{
#include<stdio.h>
int idCount = 0;
int keyWordCount = 0;
int opCount = 0;
%}

digit[0-9]
letter[a-zA-Z]

/* Rules  */
%%

[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/] {
    /* Ignores comments as comments are not tokens of a language */
}

"int"|"if"|"then"|"else"|"endif"|"while"|"do"|"enddo"|"print" {
    keyWordCount++;
    printf("%s is a Keyword\n",yytext);
}

{letter}({letter}|{digit})* {
    idCount++;
    printf("%s is an Identifier\n",yytext);
}

"<"|"<="|">"|">="|"=="|"="|"!="|"+"|"-"|"*"|"/"|"|" {
    opCount++;
    printf("%s is an Operator\n",yytext);
}

. ;

%%

/* User Routines */

int yywrap(){}

int main()
{
	yylex();
	printf("\nCount of identifiers: %d\n", idCount);
	printf("Count of keywords: %d\n", keyWordCount);
	printf("Count of operators: %d\n", opCount);
	return 0;
}
