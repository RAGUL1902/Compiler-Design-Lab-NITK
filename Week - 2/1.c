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
                        printf("   ");
                        follow(temp3, production[i][0]);

                        for (k = 0; temp3[k] != '\0'; k++)
                        {
                            add_res(array, temp3[k]);
                        }
                    }
                }
                if (production[i][j + 1] == '\0' && ch != production[i][0])
                {
                    printf("   ");
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

int main()
{
    char op;
    int ft;
    int count;
    printf("\nEnter the Total Number of Productions:\t");
    scanf("%d", &pno);
    for (count = 0; count < pno; count++)
    {
        printf("\n Production  %d:\t", count + 1);
        scanf("%s", production[count]);
    }

    do
    {
        printf("\nPress 1 for finding first and Press 2 for finding Follow: ");
        scanf("%d", &ft);
        if (ft == 1)
        {

            char ch;
            char array[25];

            printf("\nEnter a value to find first:\t");
            scanf(" %c", &ch);
            first(array, ch);
            printf("\nFirst Value of %c:\t{ ", ch);
            for (count = 0; array[count] != '\0'; count++)
            {
                printf(" %c ", array[count]);
            }
        }
        else
        {

            char ch;
            char array[25];

            printf("\nEnter a value to find follow:\t");
            scanf(" %c", &ch);
            follow(array, ch);
            printf("\nFollow Value of %c:\t{ ", ch);

            for (count = 0; array[count] != '\0'; count++)
            {
                printf(" %c ", array[count]);
            }
        }
        printf("}\n");
        printf("Press 1 to continue, else press anything \t");
        scanf(" %c", &op);
    } while (op == '1');

    return 0;
}
