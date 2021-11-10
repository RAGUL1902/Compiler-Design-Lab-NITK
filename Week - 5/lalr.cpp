#include<bits/stdc++.h>

using namespace std;

//Struct for Production contains a string and the length
struct production
{
	//String of production and length
	string s;
	int len;
	
	//Constructors
	production() {
		s = "";
		len = 0;
	}
	production ( string t ) {
		s = t;
		len = s.size();
	}
};

//Struct for State contains a vector of strings and a mapping corresponding to a transition to a different state
struct states
{
	//All the productions
	vector<pair<production, char>> items;
	
	// list of out-edges from our state
	map<char, int> gotos;
	
	//Constructors
	states() {}
	states ( vector<pair<production, char>> _items ) {
		for ( auto x : _items ) { items.push_back ( x ); }
	}
	
	//To check if a production is already present in the State
	bool isThere ( pair<production, char> prod ) {
		for ( auto x : items ) {
			if ( prod.first.s == x.first.s and prod.second == x.second ) { return true; }
		}
		return false;
	}
};

//ASCII terminals
const int SZ = 128;

//Maximum number of states
const int MX_STATES = 1000;

//Global Vars
//List of productions
vector<production> prod;

//List of States
vector<states> stateList;

//List of States for LALR
vector<states> LALRList;

//Number of productions
int n_prod = 0;

//First and follow for all non terminals
vector<vector<bool>> follow ( 26, vector<bool> ( SZ ) ), first ( 26, vector<bool> ( SZ ) );

//Boolean array to check if a non terminal is in the productions or not
vector<bool> non_terminal ( 26 );

//Boolean array to check if a terminal is in the production set
vector<bool> terminal ( 1000 );

//A global mapping of (string => stateNo)
//Direct mapping for a whole production
map<pair<string, char>, int> globalGoto;

//Action table
vector<vector<string>> ACTION ( MX_STATES, vector<string> ( 1000 ) );

//Goto table
vector<vector<int>> GOTO ( MX_STATES, vector<int> ( 26, -1 ) );

//MAP vector of prod => new state number
vector<set<string>>  newStateMap;

//MAP old state => new state
map<int, int> oldToNew;

//Function to check if a character is non terminal
bool is_non_terminal ( char ch ) { return 'A' <= ch and ch <= 'Z'; }

//Function to add the first(A) to first(B)
void union_first_A_first_B ( char A, char B ) {
	for ( int i = 0; i < SZ; i++ ) {
		if ( i == '!' ) { continue; }
		first[B - 'A'][i] = ( first[B - 'A'][i] or first[A - 'A'][i] );
	}
}

//Function to find first() for all non terminals
void find_first() {
	// cout << "first is done";
	for ( int it = 0; it < n_prod; it++ ) {
		for ( int i = 1; i < n_prod; i++ ) {
			char lhs = prod[i].s[0];
			int j = 2;
			while ( j < prod[i].len ) {
				char cur = prod[i].s[j];
				if ( is_non_terminal ( cur ) ) {
					union_first_A_first_B ( cur, lhs );
					if ( first[cur - 'A']['!'] ) {
						j++;
						continue;
					}
				}
				else {
					first[lhs - 'A'][cur] = true;
				}
				break;
			}
			if ( j == prod[i].len ) { first[lhs - 'A']['!'] = true; }
		}
	}
}

//Function to add first(A) to follow(B)
void union_first_A_follow_B ( char A, char B ) {
	for ( int i = 0; i < SZ; i++ ) {
		if ( i == '!' ) { continue; }
		follow[B - 'A'][i] = follow[B - 'A'][i] or first[A - 'A'][i];
	}
}

//Function to add follow(A) to follow(B)
void union_follow_A_follow_B ( char A, char B ) {
	for ( int i = 0; i < SZ; i++ ) {
		if ( i == '!' ) { continue; }
		follow[B - 'A'][i] = follow[B - 'A'][i] or follow[A - 'A'][i];
	}
}

//Function to find follow() for all non terminals
void find_follow() {
	// cout << "follow is done";
	for ( int it = 0; it < n_prod; it++ ) {
		for ( int cur = 0; cur < 26; cur++ ) {
			if ( !non_terminal[cur] ) { continue; }
			char ch = 'A' + cur;
			for ( int i = 1; i < n_prod; i++ ) {
				for ( int j = 2; j < prod[i].len; j++ ) {
					if ( ch != prod[i].s[j] ) { continue; }
					int k;
					for ( k = j + 1; k < prod[i].len; k++ ) {
						char next = prod[i].s[k];
						if ( is_non_terminal ( next ) ) {
							union_first_A_follow_B ( next, ch );
							if ( first[next - 'A']['!'] ) { continue; }
						}
						else {
							follow[ch - 'A'][next] = true;
						}
						break;
					}
					if ( k == prod[i].len ) {
						union_follow_A_follow_B ( prod[i].s[0], ch );
					}
				}
			}
		}
	}
}

//calculates the first of (beta)a
void first_gen ( string s, set<char> &x ) {
	for ( int i = 0; i < ( int ) s.size(); i++ ) {
		char c = s[i];
		if ( !is_non_terminal ( c ) ) {
			x.insert ( c );
			return;
		}
		for ( int j = 0; j < SZ; j++ ) {
			if ( ( char ) j != '!' and first[c - 'A'][j] ) {
				x.insert ( ( char ) j );
			}
		}
		if ( !first[c - 'A']['!'] ) {
			return;
		}
	}
	x.insert ( '!' );
}


//Print all the possible LR(0) items from a given Grammer
void print_all_items() {
	// go over all possible positions for a .
	//Check the corner case A=! carefully
	for ( int i = 0; i < n_prod; i++ ) {
		if ( prod[i].s[2] == '!' and prod[i].len == 3 ) {
			string temp = prod[i].s;
			temp.pop_back();
			temp += '.';
			cout << temp << "\n";
			continue;
		}
		for ( int j = 2; j <= prod[i].len; j++ ) {
			if ( prod[i].s[j - 1] == '\'' ) { continue; }
			string temp = prod[i].s;
			temp.insert ( j, "." );
			cout << temp << "\n";
		}
	}
}

// Initialize state0
void init_first_state() {
	vector<pair<production, char>> temp;
	//For all productions place the . at the beggining of RHS
	//Take care of A=! case separately
	// for ( auto x : prod ) {
	// 	string ins = x.s;
	// 	if ( ins[2] == '!' and ins.size() == 3 ) {
	// 		ins.pop_back();
	// 		ins += '.';
	// 	}
	// 	else { ins.insert ( 2, "." ); }
	// 	temp.push_back ( ins );
	// }
	string ins = prod[0].s;
	ins.insert ( 2, "." );
	temp.push_back ( {ins, '$'} );
	stateList.push_back ( temp );
}

// Closure of a particular when we have been given the symbol next to the .
// if the  next symbol is terminal continue
// Else we look for productions that start with next and insert . at the beggining of RHS
// A = ( alpha ).B ( beta ), a
// next = B
// suff = ( beta ) a
void closure ( char next,  string suff, states &state ) {
	if ( !isupper ( next ) ) { return; }
	set<char> first_suff;
	first_gen ( suff, first_suff );
	for ( int i = 0; i < ( int ) prod.size(); i++ ) {
		//Check for productions that start with next
		//Put a . at the beggining of RHS and push it into the state if it is not present
		if ( prod[i].s[0] == next ) {
			string aug = prod[i].s;
			if ( aug[2] == '!' and aug.size() == 3 ) {
				aug.pop_back();
				aug += '.';
			}
			else { aug.insert ( 2, "." ); }
			for ( auto ch : first_suff ) {
				pair<production, char> aug_pair = {aug, ch};
				if ( !state.isThere ( aug_pair ) ) {
					state.items.push_back ( aug_pair );
				}
			}
		}
	}
}

// This fuction constructs the states
void construct_states ( int id ) {
	cout << "S" << id << ":\n";
	// ensure that the current item contains the full closure of it's productions
	for ( int i = 0; i < ( int ) stateList[id].items.size(); i++ ) {
		string temp = stateList[id].items[i].first.s;
		char lookA = stateList[id].items[i].second;
		if ( temp.back() == '.' ) { continue; }
		int nextInd = temp.find ( '.' ) + 1;
		char next = temp[nextInd];
		string suff = "";
		if ( nextInd < temp.size() ) { suff = temp.substr ( nextInd + 1 ); }
		closure ( next, suff + lookA, stateList[id] );
	}
	// Iterate over all the items in the particular state
	for ( int i = 0; i < ( int ) stateList[id].items.size(); i++ ) {
		string pro = stateList[id].items[i].first.s;
		char lookA = stateList[id].items[i].second;
		//If the . is at the end the print and continue
		if ( pro.back() == '.' ) {
			printf ( "\t%s , %-20c\n", &pro[0], lookA );
			continue;
		}
		//Next = character after the .
		int nextInd = pro.find ( '.' ) + 1;
		assert ( nextInd < pro.size() );
		char next = pro[nextInd];
		//If the state doesn't have the 'Next' transition yet
		if ( stateList[id].gotos.find ( next ) == stateList[id].gotos.end() ) {
			// if there is a global goto defined for the entire production, use
			// that one instead of creating a new one
			if ( globalGoto.find ( {pro, lookA} ) == globalGoto.end() )
			{
				// create new state and populate it
				stateList.push_back ( states() );
				//newpro is just the pro but the . shifted 1 place to the right
				string newpro = pro;
				int pos = newpro.find ( '.' );
				assert ( pos + 1 < pro.size() );
				swap ( newpro[pos], newpro[pos + 1] );
				stateList.back().items.push_back ( {newpro, lookA} );
				//Populate global and state gotos
				stateList[id].gotos[next] = stateList.size() - 1;
				globalGoto[ {pro, lookA}] = stateList.size() - 1;
			}
			else {
				// use existing global item
				stateList[id].gotos[next] = globalGoto[ {pro, lookA}];
			}
			//Print the production and it's goto
			printf ( "\t%s , %-20c goto(%c)=S%d\n", &pro[0], lookA, next, globalGoto[ {pro, lookA}] );
		}
		//Goto exist then go to that particular state
		else {
			int pos = pro.find ( '.' );
			assert ( pos + 1 < pro.size() );
			swap ( pro[pos], pro[pos + 1] );
			// add production to next item if it doesn't already contain it
			int nextStateIndex = stateList[id].gotos[next];
			if ( !stateList[nextStateIndex].isThere ( {pro, lookA} ) ) {
				stateList[nextStateIndex].items.push_back ( {pro, lookA} );
			}
			swap ( pro[pos], pro[pos + 1] );
			printf ( "\t%s , %-20c\n", &pro[0], lookA );
		}
	}
}

// struct compp {
// 	bool operator() ( production &a, production &b ) {
// 		return a.len < b.len;
// 	}
// };

void makeLALRStates() {
	for ( int i = 0; i < stateList.size(); i++ ) {
		set<string> core;
		for ( auto [p, lookA] : stateList[i].items ) {
			core.insert ( p.s );
		}
		bool flag = false;
		for ( int j = 0; j < newStateMap.size(); j++ ) {
			if ( newStateMap[j] == core ) {
				for ( auto item : stateList[i].items ) {
					if ( !LALRList[j].isThere ( item ) ) {
						LALRList[j].items.push_back ( item );
					}
				}
				oldToNew[i] = j;
				flag = true;
				break;
			}
		}
		if ( flag ) { continue; }
		oldToNew[i] = LALRList.size();
		LALRList.push_back ( states() );
		LALRList.back().items = stateList[i].items;
		newStateMap.push_back ( core );
	}
	for ( int i = 0; i < stateList.size(); i++ ) {
		// for ( int j = 0; j < SZ; j++ ) {
		// 	if ( stateList[i].gotos.count ( ( char ) j ) ) {
		// 		int curr_state = oldToNew[i];
		// 		int nextInd = oldToNew[stateList[i].gotos[ ( char ) j]];
		// 		LALRList[curr_state].gotos[ ( char ) j] = nextInd;
		// 	}
		// }
		int curr_state = oldToNew[i];
		for ( auto x : stateList[i].gotos ) {
			int nextInd = oldToNew[x.second];
			LALRList[curr_state].gotos[x.first] = nextInd;
		}
	}
	// for ( auto x : oldToNew ) { cout << x.first << " " << x.second << endl; }
	// for ( int i = 0; i < LALRList.size(); i++ ) {
	// 	for ( auto x : LALRList[i].gotos ) {
	// 		cout << i << " " << x.first << " " << x.second << endl;
	// 	}
	// }
}

// Reading the input and inserting the extra production '=StartSymbol
void read_input() {
	//number of productions
	cin >> n_prod;
	//dummy production to be replaced by augumented production
	prod.emplace_back ( "dummy" );
	//taking input
	for ( int i = 0; i < n_prod; i++ ) {
		string t;
		cin >> t;
		for ( char ch : t ) {
			if ( 'A' <= ch and ch <= 'Z' ) {
				non_terminal[ch - 'A'] = true;
			}
			else
				if ( ch != '=' ) { terminal[ch] = true; }
		}
		prod.emplace_back ( t );
	}
	//insert augumented productions
	//Removing the dummy production
	string aug;
	aug += "'=";
	aug += prod[1].s[0];
	prod.emplace_back ( aug );
	swap ( prod[0], prod.back() );
	prod.pop_back();
	n_prod = prod.size();
	follow[prod[1].s[0] - 'A']['$'] = true;
}

void print_grammer() {
	cout << "Augumented Grammer\n";
	cout << "------------------\n";
	for ( auto x : prod ) {
		cout << x.s << endl;
	}
	cout << "------------------\n\n\n\n";
}

int index_of_prod ( string p ) {
	for ( int i = 0; i < n_prod; i++ ) {
		if ( prod[i].s == p ) { return i; }
	}
	return -1;
}

//Function to build actions table
void build_actions ( vector<states> &vs ) {
	for ( int i = 0; i < vs.size(); i++ ) {
		states cur = vs[i];
		for ( auto [p, lookA] : cur.items ) {
			if ( p.s.back() == '.' ) {
				if ( p.s[0] == '\'' and p.s[2] == prod[1].s[0] and lookA == '$' ) {
					ACTION[i]['$'] = "AC";
				}
				else {
					string temp_prod = p.s;
					temp_prod.pop_back();
					if ( temp_prod.back() == '=' ) { temp_prod += "!"; }
					string in = to_string ( index_of_prod ( temp_prod ) );
					if ( in == "-1" ) {
						cout << temp_prod << " Error!!";
						return;
					}
					// for ( int ch = 0; ch < SZ; ch++ ) {
					// if ( follow[ ( p.s[0] == '\'' ? prod[1].s[0] : p.s[0] ) - 'A'][ch] ) {
					ACTION[i][lookA] = "R" + in;
					// }
					// }
				}
				continue;
			}
			char next = p.s[p.s.find ( '.' ) + 1];
			if ( is_non_terminal ( next ) ) { continue; }
			string j = to_string ( cur.gotos[next] );
			ACTION[i][next] = "S" + j;
		}
	}
	terminal['$'] = true;
}


//Function to print the actions table
void print_actions ( vector<states> &vs ) {
	cout << "\n\nACTIONS\n";
	cout << " \t";
	for ( int i = 0; i < SZ; i++ ) {
		if ( terminal[i] and i != '!' ) { cout << ( char ) i << " \t"; }
	}
	cout << '\n';
	for ( int i = 0; i < vs.size(); i++ ) {
		cout << i << '\t';
		for ( int j = 0; j < SZ; j++ ) {
			if ( !terminal[j] or j == '!' ) { continue; }
			if ( ACTION[i][j] != "" ) {
				cout << ACTION[i][j] << '\t';
			}
			else {
				cout << "  \t";
			}
		}
		cout << '\n';
	}
}

void build_goto ( vector<states> &vs ) {
	for ( int i = 0; i < vs.size(); i++ ) {
		for ( char ch = 'A'; ch <= 'Z'; ch++ ) {
			if ( vs[i].gotos.count ( ch ) ) {
				GOTO[i][ch - 'A'] = vs[i].gotos[ch];
			}
		}
	}
}

void print_gotos ( vector<states> &vs ) {
	cout << "\n\nGOTO\n";
	cout << " \t";
	for ( int i = 0; i < 26; i++ ) {
		if ( non_terminal[i] ) {
			cout << ( char ) ( i + 'A' ) << '\t';
		}
	}
	cout << '\n';
	for ( int i = 0; i < vs.size(); i++ ) {
		cout << i << '\t';
		for ( int j = 0; j < 26; j++ ) {
			if ( non_terminal[j]  == false ) { continue; }
			if ( GOTO[i][j] != -1 ) {
				cout << GOTO[i][j] << '\t';
			}
			else {
				cout << " \t";
			}
		}
		cout << '\n';
	}
}

void parse() {
	string input;
	cin >> input;
	input += "$";
	int ptr = 0;
	stack<int> stateStack;
	stateStack.push ( 0 );
	stack<char> expStack;
	expStack.push ( '$' );
	vector<int> output;
	while ( true ) {
		string act = ACTION[stateStack.top()][input[ptr]];
		if ( act[0] == 'S' ) {
			int statenumber = stoi ( act.substr ( 1 ) );
			stateStack.push ( statenumber );
			expStack.push ( input[ptr] );
			ptr++;
		}
		else
			if ( act[0] == 'R' ) {
				int prodnumber = stoi ( act.substr ( 1 ) );
				output.push_back ( prodnumber );
				string curprod = prod[prodnumber].s;
				int rep = curprod.size() - 2;
				if ( curprod.back() == '!' ) { rep--; }
				for ( int i = 0; i < rep; i++ ) {
					expStack.pop();
					stateStack.pop();
				}
				char newtop = curprod[0];
				stateStack.push ( GOTO[stateStack.top()][newtop - 'A'] );
				expStack.push ( newtop );
			}
			else
				if ( act == "AC" ) {
					cout << "Accepted\n";
					break;
				}
				else {
					cout << "Error\n";
					return;
				}
	}
	cout << "The productions used are as follows\n";
	while ( output.size() ) {
		cout << prod[output.back()].s << '\n';
		output.pop_back();
	}
}

void printLALRStates() {
	cout << "\n\n";
	for ( int i = 0; i < LALRList.size(); i++ ) {
		cout << "S" << i << ":" << endl;
		for ( auto [pro, lookA] : LALRList[i].items ) {
			cout << "\t" << pro.s << " , " << lookA << endl;
		}
		cout << "--------------------------------\n";
	}
}

//A=(alpha).B(beta),a
int main()
{
	//reads the input and adds the '=Start production
	//Printing
	read_input();
	//Finding first() of non terminals to help find follow()
	follow[prod[1].s[0] - 'A']['$'] = true;
	find_first();
	find_follow();
	//Print the grammer with the new production
	print_grammer();
	// Initialize the first state with . at the first position
	init_first_state();
	// Print the States
	// If stateno === statelist.size() => We have nothing more to add
	cout << "The states and their transitions are as follows:\n\n";
	cout << "------------------------------------------------\n";
	int stateno = 0;
	// cout << stateList[stateno].items[0].first.s;
	while ( stateno < int ( stateList.size() ) ) {
		construct_states ( stateno );
		cout << "------------------------------------------------\n";
		stateno++;
	}
	//build the LALR State List
	makeLALRStates();
	//print the states
	printLALRStates();
	// build the actions table
	build_actions ( LALRList );
	// print actions table
	print_actions ( LALRList );
	// build the goto table
	build_goto ( LALRList );
	// print goto table
	print_gotos ( LALRList );
	//parse
	parse();
	return 0;
}
