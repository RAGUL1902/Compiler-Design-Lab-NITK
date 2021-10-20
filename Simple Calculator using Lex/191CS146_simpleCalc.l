/*
 * A lex program to develop a calculator.
 */

/* Declarations */

%{
#include<stdio.h>
int operator = 0;
int i;
float num1, num2;
%}

digit [0-9]+|([0-9]*)"."([0-9]+)
add "+"
subtract "-"
multiply "*"
divide "/"
power "^"
newLine \n

/* Rules  */

%%
{digit} {
    if(operator==0)
    {
        num1=atof(yytext); 
    }

    else
    {
        num2=atof(yytext);
        switch(operator)
        {
            case 1:num1=num1+num2;
            break;
  
            case 2:num1=num1-num2;
            break;
   
            case 3:num1=num1*num2;
            break;
   
            case 4:num1=num1/num2;
            break;
   
            case 5:
            for(i=num1;num2>1;num2--)
            num1=num1*i;
            break;
        }
        operator=0;
    }

} 
{add} {
    operator=1;
}
{subtract} {
    operator=2;
}
{multiply} {
    operator=3;
}
{divide} {
    operator=4;
}
{power} {
    operator=5;
}
{newLine} {
    printf("Output: %f\n\n",num1);
    printf("Enter an arithmetic expression: ");
}

%%

/* User Routines */

int yywrap(){ return 1; };

int  main()
{
    printf("Enter an arithmetic expression: ");
    yylex();
    return 0;
}
  

