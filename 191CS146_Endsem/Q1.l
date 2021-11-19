/*
 * There is a need for converting an old code for a new version of the compiler. 
 * In the old code we have used "%" for pointers. 
 * But now we have to replace each "%" with a "$". 
 * But this replacement shouldn't be done inside comments. 
 * Write a program to replace this old pointer with new pointer.
 */

/* Declarations */

%{
#include<stdio.h>
%}


/* Rules  */
%%

[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/] {
    // Multiline comments should be printed as it is.
    printf("%s",yytext);
}

\/\/(.*) {
    // Single line comments should be printed as it is.
    printf("%s",yytext);
}

"\n" {
    // Newlines should also be printed
    printf("\n");
}

"%" {
    // Changing the % character to $ as given in the question
    printf("$");
}

. {
    // Rest of the characters can be printed as it is.
    printf("%s",yytext);
};

%%

/* User Routines */

int yywrap(){}

int main()
{
    yyin = fopen("input.txt","r");
	yylex();
	return 0;
}
