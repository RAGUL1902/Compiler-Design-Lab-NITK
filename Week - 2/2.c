#include <stdio.h>
#include <ctype.h>
#include <string.h>

int pno;
char production[25][25];

void add_res(char array[], char val)
{
    int temp;
    for (temp = 0; array[temp] != '\0'; temp++)
    {
        if (array[temp] == val)
        {
            return;
        }
    }
    array[temp] = val;
    array[temp + 1] = '\0';
}

void first(char *array, char ch)
{
    int count, j, k;
    char temporary_result[20];
    int x;
    temporary_result[0] = '\0';
    array[0] = '\0';
    if (!(isupper(ch)))
    {
        add_res(array, ch);
        return;
    }
    for (count = 0; count < pno; count++)
    {
        if (production[count][0] == ch)
        {
            if (production[count][2] == '#')
            {
                add_res(array, '#');
            }
            else
            {
                j = 2;
                while (production[count][j] != '\0')
                {
                    x = 0;
                    first(temporary_result, production[count][j]);
                    for (k = 0; temporary_result[k] != '\0'; k++)
                    {
                        if (temporary_result[k] == '#')
                        {
                            continue;
                        }
                        add_res(array, temporary_result[k]);
                    }
                    for (k = 0; temporary_result[k] != '\0'; k++)
                    {
                        if (temporary_result[k] == '#')
                        {
                            x = 1;
                            break;
                        }
                    }
                    if (!x)
                    {
                        break;
                    }
                    j++;
                }
                if (production[count][j] == '\0')
                {
                    add_res(array, '#');
                }
            }
        }
    }
    return;
}

void follow(char *array, char ch)
{
    int i, j, k;
    int length = strlen(production[i]);
    array[0] = '\0';
    char temporary_result[20];
    char temp2[20];
    char temp3[20];

    if (production[0][0] == ch)
    {
        add_res(array, '$');
    }

    for (i = 0; i < pno; i++)
    {
        length = strlen(production[i]);
        for (j = 2; j < length; j++)
        {
            if (production[i][j] == ch)
            {
                if (production[i][j + 1] != '\0')
                {
                    int t = 0, z;
                    z = j;
                    while (production[i][z + 1] != '\0')
                    {
                        first(temporary_result, production[i][z + 1]);

                        for (k = 0; temporary_result[k] != '\0'; k++)
                        {
                            if (temporary_result[k] == '#')
                            {
                                t = 1;
                                continue;
                            }
                            add_res(array, temporary_result[k]);
                        }
                        if (!t)
                        {
                            break;
                        }

                        z++;
                    }
                    if (production[i][z + 1] == '\0' && production[i][z + 1] != production[i][0])
                    {
                        //printf("hey");
                        follow(temp3, production[i][0]);

                        for (k = 0; temp3[k] != '\0'; k++)
                        {
                            add_res(array, temp3[k]);
                        }
                    }
                }
                if (production[i][j + 1] == '\0' && ch != production[i][0])
                {
                    //printf("hey");
                    follow(temp2, production[i][0]);

                    for (k = 0; temp2[k] != '\0'; k++)
                    {
                        add_res(array, temp2[k]);
                    }
                }
            }
        }
    }
}

void pro_first(char *array, int s)
{
    array[0] = '\0';
    char temp_res[20];
    int j, k, x;
    if (production[s][2] == '#')
    {
        add_res(array, '#');
    }
    else
    {
        j = 2;
        while (production[s][j] != '\0')
        {
            x = 0;
            first(temp_res, production[s][j]);
            for (k = 0; temp_res[k] != '\0'; k++)
            {
                if (temp_res[k] == '#')
                {
                    continue;
                }
                add_res(array, temp_res[k]);
            }
            for (k = 0; temp_res[k] != '\0'; k++)
            {
                if (temp_res[k] == '#')
                {
                    x = 1;
                    break;
                }
            }
            if (!x)
            {
                break;
            }
            j++;
        }
        if (production[s][j] == '\0')
        {
            add_res(array, '#');
        }
    }
}

int main()
{
    int count, i, j, len, k, l, ct1 = 0, ct2 = 0;

    printf("\nEnter the Total Number of Productions:\t");
    scanf("%d", &pno);
    for (count = 0; count < pno; count++)
    {
        printf("\n Production  %d:\t", count + 1);
        scanf("%s", production[count]);
    }

    char terminals[40], nonterminals[40], ch;
    terminals[0] = '\0';
    nonterminals[0] = '\0';
    for (i = 0; i < count; i++)
    {
        len = strlen(production[i]);
        for (j = 0; j < len; j++)
        {
            if (j != 1)
            {
                ch = production[i][j];
                if (isupper(ch))
                {
                    add_res(nonterminals, ch);
                }
                else
                {
                    if (ch == '#')
                    {
                        continue;
                    }
                    add_res(terminals, ch);
                }
            }
        }
    }
    for (i = 0; i < nonterminals[i] != '\0'; i++)
    {
    }

    ct2 = i;
    // printf("%d ", ct2);
    for (i = 0; i < terminals[i] != '\0'; i++)
    {
    }
    terminals[i] = '$';
    terminals[i + 1] = '\0';
    ct1 = i + 1;
    //printf("%d ",ct1);
    // printf("hey1 ");
    int parsetable[ct2][ct1];
    for (i = 0; i < ct2; i++)
    {
        for (j = 0; j < ct1; j++)
        {
            parsetable[i][j] = 0;
        }
    }
    for (i = 0; i < count; i++)
    {
        char tem1[20], tem2[20];
        pro_first(tem1, i);
        for (j = 0; j < ct2; j++)
        {
            if (nonterminals[j] == production[i][0])
            {
                l = j;
                break;
            }
        }
        //printf("hey2")
        int x = 0;
        for (k = 0; tem1[k] != '\0'; k++)
        {
            if (tem1[k] == '#')
            {
                x = 1;
                continue;
            }
            for (j = 0; j < ct1; j++)
            {
                if (terminals[j] == tem1[k])
                {
                    parsetable[l][j] = i + 1;
                }
            }
        }

        if (x)
        {
            follow(tem2, production[i][0]);
            for (k = 0; tem2[k] != '\0'; k++)
            {
                for (j = 0; j < ct1; j++)
                {
                    if (terminals[j] == tem2[k])
                    {
                        parsetable[l][j] = i + 1;
                    }
                }
            }
        }
    }

    printf("\n\t       Parsing table ");
    printf("\n\t");
    for (i = 0; i < ct1; i++)
    {
        printf("%c", terminals[i]);
        printf("\t");
    }
    printf("\n");
    for (i = 0; i < ct2; i++)
    {
        printf("%c", nonterminals[i]);
        printf("\t");
        for (j = 0; j < ct1; j++)
        {
            int z;
            z = parsetable[i][j];
            if (z == 0)
            {
                printf("\t");
            }
            else
            {
                printf("%s", production[z - 1]);
                printf("\t");
            }
        }
        printf("\n");
    }

    return 0;
}