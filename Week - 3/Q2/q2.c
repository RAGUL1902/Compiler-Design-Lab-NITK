#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char items[30][100][100];
char augmented_grammar[100][100], terminals[30], nonterminals[30];
int nop = 0, states = 0, no_of_items[25], n_t = 0, n_nt = 0;


int state_index = 0, goto_state_index = 0, closure_item_index = 0;

int check(char c) {
	int i;

	for(i = 0; i < n_t; i++)
		if(terminals[i] == c)
			return 1;

	return 0;
}

void generate_terminals() {
	int i, j;
	int idx = 0;

	for(i = 0; i < nop; i++) {
		for(j = 0; augmented_grammar[i][j] != '>'; j++);
		j++;

		for(; augmented_grammar[i][j] != '\0'; j++) {
			if(augmented_grammar[i][j] < 65 || augmented_grammar[i][j] > 90) {
				if(!check(augmented_grammar[i][j])) {
					terminals[idx] = augmented_grammar[i][j];
					n_t++;
					idx++;
				}
			}
		}
	}

	terminals[idx] = '$';
    n_t++;
	idx++;
	terminals[idx] = '\0';
}

int check2(char c, int index) {
	int i;

	for(i = 0; i < index; i++)
		if(nonterminals[i] == c)
			return 1;

	return 0;
}

void generate_nonterminals() {
	int i, idx = 0;

	for(i = 0; i < nop; i++)
		if(!check2(augmented_grammar[i][0], idx)) {
			nonterminals[idx] = augmented_grammar[i][0];
			idx++;
		}

    n_nt= idx;
	nonterminals[idx] = '\0';
}

void initialize_items() {
	generate_terminals();
	generate_nonterminals();

	int i;

	for(i = 0; i < 30; i++)
		no_of_items[i] = 0;
}

void generate_item(char *s, char *t) {
	int i;

	for(i = 0; i < 3; i++)
		t[i] = s[i];

	t[i] = '.';

	if(s[i] != '@')
		for(; i < strlen(s); i++)
			t[i+1] = s[i];

	t[i+1] = '\0';
}

int item_found(char *s) {	//Check for items in a state.
	int i;

	for(i = 0; i < closure_item_index; i++) {
		if(!strcmp(s, items[state_index][i]))	//If the strings match.
			return 1;
	}

	return 0;
}

int isterminal(char s) {
	int i;

	for(i = 0; i < n_t; i++)
		if(s == terminals[i])
			return 1;

	return 0;
}

void closure(char *s) {
	int i, j;

	for(i = 0; s[i] != '.'; i++);

	i++;

	if(!item_found(s)) {
		strcpy(items[state_index][closure_item_index], s);
		closure_item_index++;

//		printf("%s\n", items[state_index][closure_item_index-1]);
	}

	if(s[i] == s[0] && s[i-2] == '>')	//To avoid infinite loop due to left recursion.
		return;

	if(isterminal(s[i]))
		return;

	else 	{	//Not a terminal(non-terminal)
		for(j = 0; j < nop; j++) {
			char temp[100];

			if(augmented_grammar[j][0] == s[i]) {
				generate_item(augmented_grammar[j], temp);
				closure(temp);
			}
		}
	}
}

int Goto(char s, char temp[][100]) {	//Find Goto on symbol s. GOTO(goto_state_index, s)
	int i, j;
	int n = 0;
	char t, temp2[100];

	if(s == '\0') {
		return n;
	}

	for(i = 0; i < no_of_items[goto_state_index]; i++) {
		strcpy(temp2, items[goto_state_index][i]);

		for(j = 0; temp2[j] != '.'; j++);

		if(temp2[j+1] == '\0')
			continue;

		if(temp2[j+1] == s) {
			t = temp2[j];
			temp2[j] = temp2[j+1];
			temp2[j+1] = t;

			strcpy(temp[n], temp2);
			n++;
		}
	}

	return n;
}
int state_found(char *s) {	//Checks for existance of same state.
	int i;

	for(i = 0; i < state_index; i++) {
		if(!strcmp(s, items[i][0]))	//Compare with the first item of each state.
			return 1;
	}

	return 0;
}

int transition_item_found(char * t_items, char s, int t_index) {
	int i;

	for(i = 0; i < t_index; i++)
		if(s == t_items[i])
			return 1;

	return 0;
}

void compute_closure_goto() {
	char temp[100][100], transition_items[100];
	int i, no_of_goto_items,j, transition_index = 0;
	char temporary[100][100];

	generate_item(augmented_grammar[0], temp[0]);//(Z->S)=>(Z->.S)
	closure(temp[0]);//S->.AA, A->.aA, A->.b

	no_of_items[state_index] = closure_item_index;
	closure_item_index = 0;

	state_index++;
	//state_index is 1 now.

	while(goto_state_index < 30) {
		transition_index = 0;
		transition_items[transition_index] = '\0';

		for(i = 0; i < no_of_items[goto_state_index]; i++) {
			for(j = 0; items[goto_state_index][i][j] != '.'; j++);
			j++;

			if(!transition_item_found(transition_items, items[goto_state_index][i][j], transition_index)) {
				transition_items[transition_index] = items[goto_state_index][i][j];
				transition_index++;
			}
		}


		transition_items[transition_index] = '\0';


		for(i = 0; i < transition_index; i++) {
			int add_flag = 0;
			no_of_goto_items = Goto(transition_items[i], temp);
			for(j = 0; j < no_of_goto_items; j++) {
				if(!state_found(temp[j])) {
					add_flag = 1;
					closure(temp[j]);
				}
				else
					break;
			}
			j=0;

			if(add_flag) {
				no_of_items[state_index] = closure_item_index;
				while(j<closure_item_index){
				     printf("\nFrom state %d on transition symbol of %c it goes to state %s \n\n",goto_state_index,transition_items[i],items[state_index][j]);
				       j++;

				}
				closure_item_index = 0;
				state_index++;
			}
		}

		goto_state_index++;
	}

	states = state_index;
}

void display() {
	int i, j;

	printf("\nNumber of states = %d\n", states);

	printf("\n\n State_no |\t items\n");
	printf("-------------------------------------------\n");

	for(i = 0; i < states; i++) {
		printf("\n  %d       |", i);

		for(j = 0; j < no_of_items[i]; j++)
			printf(" %s   ", items[i][j]);
        printf("\n---------------------------------------------\n");
	}

}


void function() {
	char str[100];

	printf("Enter number of productions:");
	scanf("%d", &nop);

	printf("\nEnter the productions >>>\n");

	int i;
	for(i = 1; i <= nop; i++)
    {
        printf("Production %d: ",i);
        scanf("%s", augmented_grammar[i]);
        printf("\n");
    }


	printf("\n\nAugmented Grammar of the above grammar is:\n");

	strcpy(augmented_grammar[0], "Z->");
	str[0] = augmented_grammar[1][0];
	str[1] = '\0';
	strcat(augmented_grammar[0], str);

	nop++;

	for(i = 0; i < nop; i++)
		printf("%s\n", augmented_grammar[i]);

	initialize_items();

	compute_closure_goto();

	display();





}

int main()
{
    function();

    return 0;
}
