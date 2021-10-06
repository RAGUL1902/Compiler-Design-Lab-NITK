//1. Write a program to find LR(0) items
#include <stdio.h>
#include <string.h>

char productions[50][100];
char LR_0[150][100];
int LR_len;
int main()
{
    int n;
    printf("Enter number of productions in the grammar: ");
    scanf("%d", &n);
    printf("Enter productions in the form A->B: \n");
    for (int i = 0; i < n; i++)
    {

        scanf("%s", productions[i]);
        getchar();
    }
    memset(LR_0, '!', sizeof(LR_0));
    //computing lr 0 items
    int lr_index = 0;
    for (int i = 0; i < n; i++)
    {
        int prod_len = strlen(productions[i]);
        for (int j = 3; j <= prod_len; j++)
        {
            int k;
            for (k = 0; k < j; k++)
            {
                LR_0[lr_index][k] = productions[i][k];
            }
            LR_0[lr_index][k] = '.';
            k = k + 1;
            for (k; k <= prod_len; k++)
            {
                LR_0[lr_index][k] = productions[i][k - 1];
            }
            LR_0[lr_index][k] = '\0';
            lr_index++;
            if (productions[i][j] == '#') //in this case only .# is taken and not #. so breaking after getting .#
                break;
        }
    }
    LR_len = lr_index;
    printf("LR(0) items are: \n");
    for (int i = 0; i < LR_len; i++)
    {
        printf("\t%s\n", LR_0[i]);
    }
    return 0;
}