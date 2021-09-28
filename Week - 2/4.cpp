// Extend the above program to print the Productions used while checking the acceptance of string.

#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int main()
{
    int m, n;
    cout << "Enter the number of non-terminals: ";
    cin >> n;
    cout << "Enter the number of terminals: ";
    cin >> m;

    vector<vector<pair<char, string>>> parsetable(n, vector<pair<char, string>>(m + 1));
    char terminals[n];
    char nonTerminals[m + 1];

    cout << "Enter the non-terminals" << endl;
    for (int i = 0; i < n; i++)
        cin >> nonTerminals[i];

    cout << "Enter the terminals" << endl;
    for (int i = 0; i < m; i++)
        cin >> terminals[i];

    terminals[m] = '$';
    char nonterminal;
    string production;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j <= m; j++)
        {
            cout << "Enter the production for: " << i << "," << j << endl;
            cout << "Non-terminal: ";
            cin >> nonterminal;
            cout << "Production: ";
            cin >> production;

            parsetable[i][j] = {nonterminal, production};
        }
    }

    int ans;
    cout << "Press 1 to continue the string or 0 to stop" << endl;
    cin >> ans;
    vector<pair<char, string>> parses;

    while (ans)
    {
        string input;
        string buffer;
        cout << "Enter the input-string: ";
        cin >> input;

        stack<char> st;
        bool flag = false;
        st.push('A');
        char stTop;
        int idx1, idx2, i = 0, len = input.length();

        while (i < len)
        {
            for (idx2 = 0; idx2 < m; idx2++)
                if (terminals[idx2] == input[i])
                    break;

            if (idx2 == m)
            {
                cout << "Not LL1 accepted" << endl;
                flag = true;
                break;
            }

            while (!st.empty())
            {
                stTop = st.top();

                if (stTop == input[i])
                    break;

                for (idx1 = 0; idx1 < n; idx1++)
                    if (nonTerminals[idx1] == stTop)
                        break;

                if (idx1 == n)
                {
                    flag = true;
                    break;
                }

                st.pop();
                buffer = parsetable[idx1][idx2].second;
                parses.push_back(parsetable[idx1][idx2]);
                for (int k = buffer.length() - 1; k >= 0; k--)
                    st.push(buffer[k]);
            }

            if (flag == true || st.empty())
            {
                flag = true;
                cout << "Not LL1 accepted" << endl;
                break;
            }

            st.pop();
            i++;
        }

        if (!flag)
        {

            if (!st.empty())
                cout << "Not LL1 accepted" << endl;

            else
            {
                cout << "Accepted" << endl;
                cout << "The parses were:" << endl;
                for (auto iter = parses.begin(); iter != parses.end(); iter++)
                    cout << iter->first << "->" << iter->second << endl;
            }
        }

        parses.clear();
        cout << "Press 1 to continue the string or 0 to stop" << endl;
        cin >> ans;
    }

    return 0;
}
