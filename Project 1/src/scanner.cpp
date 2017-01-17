/*
 * CSE 6341
 * Project: 1
 * File	  : scanner.cpp
 * Author : Rajarshi Biswas
 * Email  : biswas.91@osu.edu
 *
 */

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <assert.h>
#include "common.h"
#include "scanner.h"

using namespace std;

// Used to store a unparsed char. 
static bool store_flag;
static char store_char;

// Used to store all the atom values.
static vector<string> literal_atom_list;
static vector<int> numeric_atom_list;

/*
 * Checks whether the current input is end of a token or not.
 * input - a Character.
 * 
 * Returns true if end of token, otherwise returns false.
 */
static bool end_of_token (char input) {
	if (input != 32 && input != 13 && input != 10 &&
	    input != '(' && input !=')') {
		return false;
	} else {
		return true;
	}

}

/*
 * Reads stdin and generates token.
 *
 * Returns a token as soon as it finds a valid one.
 */
static token getNextToken () {
	token_type t;
	token result_token;
	char input;
	string numeric_atom;
	string literal_atom;
	string error_token;

	// Check whether we have a stored character to consider.
	if (store_flag == true) {
		input = store_char;
	} else { // Else read from stdin.
		input = getchar();
	}
	store_flag = false;

	if (input == EOF) {
		// 1. Empty file or, 
		// 2. We have scanned all the input.
		t = END_OF_FILE;
		goto end;
	}

	// Consume all the white space, carriage return and line feed
	while (input == 32 ||  input == 13 || input == 10) {
		input = getchar();
	}

	if (input == EOF) {
		// Check for the EOF.
		t =  END_OF_FILE;
		goto end;
	} else if (input == '(') {
		// Check open parenthesis token.
		t =  OPEN_PAREN;
		goto end;
	} else if (input == ')') {
		// Check closing parenthesis token.
		t = CLOSING_PAREN;
		goto end;
	} else if (isupper(input)) {
		// Check literal atom.
		literal_atom = "";
		literal_atom += input;
		input = getchar();
		while (isupper(input) || isdigit(input)) {
			literal_atom = literal_atom + input;
			input = getchar();
		}

		if (end_of_token(input) != true) {
			// Literal atom error.
			literal_atom += input;
			input = getchar();
			while (end_of_token(input) != true) {
				literal_atom += input;
				input = getchar();
			}
			error_token = literal_atom;
			t =  ERROR;
			goto end;
		}

		store_char = input;
		store_flag = true;

		// Store the literal atom.
		literal_atom_list.push_back(literal_atom);
		t = LITERAL_ATOM;
		goto end;
	} else if (isdigit(input)) {
		// Check numeric atom.
		numeric_atom = "";
		numeric_atom += input;
		input = getchar();
		while (isdigit(input)) {
			numeric_atom += input;
			input = getchar();
		}

		if (end_of_token (input) != true) {
			// Numeric atom error.
			numeric_atom += input;
			input = getchar();
			while (end_of_token(input) != true) {
				numeric_atom += input;
				input = getchar();
			}
			error_token = numeric_atom;
			t = ERROR;
			goto end;
		}

		store_char = input;
		store_flag = true;

		// Store the numeric atom.
		numeric_atom_list.push_back(stoi(numeric_atom));
		t = NUMERIC_ATOM;
		goto end;
	} else {
		// Default case. No matching tag.
		error_token = "";
		error_token += input;
		input = getchar();
		while (end_of_token(input) != true) {
			error_token += input;
			input = getchar();
		}
		t = ERROR;
		goto end;
	}

end:
	// Generates the token with type and all other information.
	result_token.type = t;
	switch (t) {
		case LITERAL_ATOM:
			result_token.value.literal = literal_atom;
			break;
		case NUMERIC_ATOM:
			result_token.value.numeric = stoi(numeric_atom);
			break;
		case ERROR:
			result_token.value.error = error_token;
			break;
		default:
			break;
	}

	return (result_token);
}

int scanner () {
	unsigned int error = SUCCESS;
	unsigned int num_literal_atom = 0;
	unsigned int num_numeric_atom = 0;
	unsigned int num_oepn_paran = 0;
	unsigned int num_closing_paran = 0;
	int total_sum = 0;

	while (true) {
		token t = getNextToken();
		switch (t.type) {
			case LITERAL_ATOM:
				num_literal_atom++;
				break;
			case NUMERIC_ATOM:
				num_numeric_atom++;
				total_sum += t.value.numeric;
				break;
			case OPEN_PAREN:
				num_oepn_paran++;
				break;
			case CLOSING_PAREN:
				num_closing_paran++;
				break;
			case END_OF_FILE:
				goto done;
				break;
			case ERROR:
				// Error in parsing. Exit.
				cout << "ERROR: Invalid token " << t.value.error;
				goto out;
				break;
			default:
				assert(0);
				break;
		}
	}

done:
	// Prints the necessary information.
	cout << "LITERAL ATOMS: " << num_literal_atom;
	for (int i = 0; i < literal_atom_list.size(); i++) {
		cout <<  ", " << literal_atom_list[i]; 
	}
	cout << "\nNUMERIC ATOMS: " << num_numeric_atom << ", " << total_sum;
	cout << "\nOPEN PARENTHESES: " << num_oepn_paran;
	cout << "\nCLOSING PARENTHESES: " << num_closing_paran;

out:
	cout << "\n";
	return (error);
}
