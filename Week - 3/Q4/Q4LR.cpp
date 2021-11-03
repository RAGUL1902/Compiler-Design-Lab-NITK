#include <bits/stdc++.h>

using namespace std;

// Struct for Production contains a string and the length
struct production
{
	// String of production and length
	string s;
	int len;

	// Constructors
	production()
	{
		s = "";
		len = 0;
	}
	production(string t)
	{
		s = t;
		len = s.size();
	}
};

// Struct for State contains a vector of strings and a mapping corresponding to a transition to a different state
struct states
{
	// All the productions
	vector<production> items;

	// list of out-edges from our state
	map<char, int> gotos;

	// Constructors
	states() {}
	states(vector<production> _items)
	{
		for (auto x : _items)
		{
			items.push_back(x);
		}
	}

	// To check if a production is already present in the State
	bool isThere(string prod)
	{
		for (auto x : items)
		{
			if (prod == x.s)
			{
				return true;
			}
		}
		return false;
	}
};

const int SZ = 128;
const int MX_STATES = 1000;
vector<production> prod;
vector<states> stateList;
int n_prod = 0;

vector<bool> non_terminal(26);

// Boolean array to check if a terminal is in the production set
vector<bool> terminal(1000);

// A global mapping of (string => stateNo)
// Direct mapping for a whole production
map<string, int> globalGoto;

// Action table
vector<vector<string>> ACTION(MX_STATES, vector<string>(1000));

// Goto table
vector<vector<int>> GOTO(MX_STATES, vector<int>(26, -1));

// Function to check if a character is non terminal
bool is_non_terminal(char ch) { return 'A' <= ch and ch <= 'Z'; }

// Print all the possible LR(0) items from a given Grammer
void print_all_items()
{
	// go over all possible positions for a .
	// Check the corner case A=! carefully
	for (int i = 0; i < n_prod; i++)
	{
		if (prod[i].s[2] == '!' and prod[i].len == 3)
		{
			string temp = prod[i].s;
			temp.pop_back();
			temp += '.';
			cout << temp << "\n";
			continue;
		}
		for (int j = 2; j <= prod[i].len; j++)
		{
			if (prod[i].s[j - 1] == '\'')
			{
				continue;
			}
			string temp = prod[i].s;
			temp.insert(j, ".");
			cout << temp << "\n";
		}
	}
}

// Initialize state0
void init_first_state()
{
	vector<production> temp;
	// For all productions place the . at the beggining of RHS
	// Take care of A=! case separately
	for (auto x : prod)
	{
		string ins = x.s;
		if (ins[2] == '!' and ins.size() == 3)
		{
			ins.pop_back();
			ins += '.';
		}
		else
		{
			ins.insert(2, ".");
		}
		temp.push_back(ins);
	}
	stateList.push_back(temp);
}

// Closure of a particular when we have been given the symbol next to the .
// if the  next symbol is terminal continue
// Else we look for productions that start with next and insert . at the beggining of RHS
void closure(char next, states &state)
{
	if (!isupper(next))
	{
		return;
	}
	for (int i = 0; i < (int)prod.size(); i++)
	{
		// Check for productions that start with next
		// Put a . at the beggining of RHS and push it into the state if it is not present
		if (prod[i].s[0] == next)
		{
			string aug = prod[i].s;
			if (aug[2] == '!' and aug.size() == 3)
			{
				aug.pop_back();
				aug += '.';
			}
			else
			{
				aug.insert(2, ".");
			}
			if (!state.isThere(aug))
			{
				state.items.push_back(aug);
			}
		}
	}
}

// This fuction constructs the states
void construct_states(int id)
{
	cout << "S" << id << ":\n";
	// ensure that the current item contains te full closure of it's productions
	for (int i = 0; i < stateList[id].items.size(); i++)
	{
		string temp = stateList[id].items[i].s;
		if (temp.back() == '.')
		{
			continue;
		}
		char next = temp[temp.find('.') + 1];
		closure(next, stateList[id]);
	}
	// Iterate over all the items in the particular state
	for (int i = 0; i < (int)stateList[id].items.size(); i++)
	{
		string pro = stateList[id].items[i].s;
		// If the . is at the end the print and continue
		if (pro.back() == '.')
		{
			printf("\t%-20s\n", &pro[0]);
			continue;
		}
		// Next = character after the .
		char next = pro[pro.find('.') + 1];
		// If the state doesn't have the 'Next' transition yet
		if (stateList[id].gotos.find(next) == stateList[id].gotos.end())
		{
			// if there is a global goto defined for the entire production, use
			// that one instead of creating a new one
			if (globalGoto.find(pro) == globalGoto.end())
			{
				// create new state and populate it
				stateList.push_back(states());
				// newpro is just the pro but the . shifted 1 place to the right
				string newpro = pro;
				int pos = newpro.find('.');
				swap(newpro[pos], newpro[pos + 1]);
				stateList.back().items.push_back(newpro);
				// Populate global and state gotos
				stateList[id].gotos[next] = stateList.size() - 1;
				globalGoto[pro] = stateList.size() - 1;
			}
			else
			{
				// use existing global item
				stateList[id].gotos[next] = globalGoto[pro];
			}
			// Print the production and it's goto
			printf("\t%-20s goto(%c)=S%d\n", &pro[0], next, globalGoto[pro]);
		}
		// Goto exist then go to that particular state
		else
		{
			int pos = pro.find('.');
			swap(pro[pos], pro[pos + 1]);
			// add production to next item if it doesn't already contain it
			int nextStateIndex = stateList[id].gotos[next];
			if (!stateList[nextStateIndex].isThere(pro))
			{
				stateList[nextStateIndex].items.push_back(pro);
			}
			swap(pro[pos], pro[pos + 1]);
			printf("\t%-20s\n", &pro[0]);
		}
	}
}

// Reading the input and inserting the extra production '=StartSymbol
void read_input()
{
	// number of productions
	cin >> n_prod;
	// dummy production to be replaced by augumented production
	prod.emplace_back("dummy");
	// taking input
	for (int i = 0; i < n_prod; i++)
	{
		string t;
		cin >> t;
		for (char ch : t)
		{
			if ('A' <= ch and ch <= 'Z')
			{
				non_terminal[ch - 'A'] = true;
			}
			else if (ch != '=')
				terminal[ch] = true;
		}
		prod.emplace_back(t);
	}
	// insert augumented productions
	// Removing the dummy production
	string aug;
	aug += "'=";
	aug += prod[1].s[0];
	prod.emplace_back(aug);
	swap(prod[0], prod.back());
	prod.pop_back();
	n_prod = prod.size();
	terminal['$'] = true;
}

void print_grammer()
{
	cout << "Augumented Grammer\n";
	cout << "------------------\n";
	for (auto x : prod)
	{
		cout << x.s << endl;
	}
	cout << "------------------\n\n\n\n";
}

int index_of_prod(string p)
{
	for (int i = 0; i < n_prod; i++)
	{
		if (prod[i].s == p)
			return i;
	}
	return -1;
}

// Function to build actions table
void build_actions()
{
	for (int i = 0; i < stateList.size(); i++)
	{
		states cur = stateList[i];
		for (production p : cur.items)
		{
			if (p.s.back() == '.')
			{
				if (p.s[0] == '\'' and p.s[2] == prod[1].s[0])
				{
					ACTION[i]['$'] = "AC";
				}
				else
				{
					string temp_prod = p.s;
					temp_prod.pop_back();
					if (temp_prod.back() == '=')
						temp_prod += "!";
					string in = to_string(index_of_prod(temp_prod));
					if (in == "-1")
					{
						cout << temp_prod << " Error!!";
						return;
					}
					for (int ch = 0; ch < SZ; ch++)
					{
						if (terminal[ch])
						{
							ACTION[i][ch] = "R" + in;
						}
					}
				}
				continue;
			}
			char next = p.s[p.s.find('.') + 1];
			if (is_non_terminal(next))
				continue;
			string j = to_string(cur.gotos[next]);
			ACTION[i][next] = "S" + j;
		}
	}
	terminal['$'] = true;
}

// Function to print the actions table
void print_actions()
{
	cout << "\n\nACTIONS\n";
	cout << " \t";
	for (int i = 0; i < SZ; i++)
	{
		if (terminal[i] and i != '!')
			cout << (char)i << " \t";
	}
	cout << '\n';
	for (int i = 0; i < stateList.size(); i++)
	{
		cout << i << '\t';
		for (int j = 0; j < SZ; j++)
		{
			if (!terminal[j] or j == '!')
				continue;
			if (ACTION[i][j] != "")
			{
				cout << ACTION[i][j] << '\t';
			}
			else
			{
				cout << "  \t";
			}
		}
		cout << '\n';
	}
}

void build_goto()
{
	for (int i = 0; i < stateList.size(); i++)
	{
		for (char ch = 'A'; ch <= 'Z'; ch++)
		{
			if (stateList[i].gotos.count(ch))
			{
				GOTO[i][ch - 'A'] = stateList[i].gotos[ch];
			}
		}
	}
}

void print_gotos()
{
	cout << "\n\nGOTO\n";
	cout << " \t";
	for (int i = 0; i < 26; i++)
	{
		if (non_terminal[i])
		{
			cout << (char)(i + 'A') << '\t';
		}
	}
	cout << '\n';
	for (int i = 0; i < stateList.size(); i++)
	{
		cout << i << '\t';
		for (int j = 0; j < 26; j++)
		{
			if (non_terminal[j] == false)
				continue;
			if (GOTO[i][j] != -1)
			{
				cout << GOTO[i][j] << '\t';
			}
			else
			{
				cout << " \t";
			}
		}
		cout << '\n';
	}
}

void parse()
{
	string input;
	cin >> input;
	input += "$";
	int ptr = 0;
	stack<int> stateStack;
	stateStack.push(0);
	stack<char> expStack;
	expStack.push('$');
	vector<int> output;
	while (true)
	{
		string act = ACTION[stateStack.top()][input[ptr]];
		if (act[0] == 'S')
		{
			int statenumber = stoi(act.substr(1));
			stateStack.push(statenumber);
			expStack.push(input[ptr]);
			ptr++;
		}
		else if (act[0] == 'R')
		{
			int prodnumber = stoi(act.substr(1));
			output.push_back(prodnumber);
			string curprod = prod[prodnumber].s;
			int rep = curprod.size() - 2;
			if (curprod.back() == '!')
				rep--;
			for (int i = 0; i < rep; i++)
			{
				expStack.pop();
				stateStack.pop();
			}
			char newtop = curprod[0];
			stateStack.push(GOTO[stateStack.top()][newtop - 'A']);
			expStack.push(newtop);
		}
		else if (act == "AC")
		{
			cout << "Accepted\n";
			break;
		}
		else
		{
			cout << "Error\n";
			return;
		}
	}
	cout << "The productions used are as follows\n";
	while (output.size())
	{
		cout << prod[output.back()].s << '\n';
		output.pop_back();
	}
}

int main()
{
	// reads the input and adds the '=Start production
	// Printing
	read_input();
	// Print the grammer with the new production
	print_grammer();
	// Initialize the first state with . at the first position
	init_first_state();
	// Print the States
	// If stateno === statelist.size() => We have nothing more to add
	cout << "The states and their transitions are as follows:\n\n";
	cout << "------------------------------------------------\n";
	int stateno = 0;
	while (stateno < int(stateList.size()))
	{
		construct_states(stateno);
		cout << "------------------------------------------------\n";
		stateno++;
	}

	// build the actions table
	build_actions();

	// print actions table
	print_actions();

	// build the goto table
	build_goto();

	// print goto table
	print_gotos();

	// parse a give string
	parse();

	return 0;
}