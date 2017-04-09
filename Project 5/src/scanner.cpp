/*
 * CSE 6341
 * Project: 2
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

/*
 * Checks whether the current input is end of a token or not.
 * input - a Character.
 *
 * Returns true if end of token, otherwise returns false.
 */
bool Scanner::end_of_token (char input) {
    if (input != 32 && input != 13 && input != 10 &&
            input != '(' && input !=')') {
        return false;
    } else {
        return true;
    }

}

/*
 * Scan and returns the next character.
 * Also, keep a Character count for bookkeeping.
 */
char Scanner::scanGetChar() {
    char input = getchar();
    this->char_count++;
    return (input);
}

/*
 * Reads stdin and generates token.
 *
 * Returns a token as soon as it finds a valid one.
 */
token Scanner::getNextToken () {
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
        input = scanGetChar();
    }
    store_flag = false;

    if (input == EOF) {
        // 1. Empty file or,
        // 2. We have scanned all the input.
        t = END_OF_FILE;
        this->global_char = input;
        goto end;
    }

    // Consume all the white space, carriage return and line feed
    while (input == 32 ||  input == 13 || input == 10) {
        if (input == 10) {
            this->line_number++;
            this->char_count = 0;
        }
        input = scanGetChar();
    }

    this->global_char = input;
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
        input = scanGetChar();
        while (isupper(input) || isdigit(input)) {
            literal_atom = literal_atom + input;
            input = scanGetChar();
        }

        if (end_of_token(input) != true) {
            // Literal atom error.
            literal_atom += input;
            input = scanGetChar();
            while (end_of_token(input) != true) {
                literal_atom += input;
                input = scanGetChar();
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
        input = scanGetChar();
        while (isdigit(input)) {
            numeric_atom += input;
            input = scanGetChar();
        }

        if (end_of_token (input) != true) {
            // Numeric atom error.
            numeric_atom += input;
            input = scanGetChar();
            while (end_of_token(input) != true) {
                numeric_atom += input;
                input = scanGetChar();
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
        input = scanGetChar();
        while (end_of_token(input) != true) {
            error_token += input;
            input = scanGetChar();
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

/*
 * Check the current found token for sanity.
 * Also, some bookkeeping about the tokens.
 */
int Scanner::checkToken(token t) {
    int error = SUCCESS;

    switch (t.type) {
        case LITERAL_ATOM:
            num_literal_atom++;
            break;
        case NUMERIC_ATOM:
            num_numeric_atom++;
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
            error = INTERPRETER_ERROR;
            goto done;
            break;
        default:
            assert(0);
            break;
    }
done:
    return (error);

}

// Get the current token
token Scanner::getCurrent() {
    return current;
}

// Get the next token
int Scanner::moveToNext() {
    int error = SUCCESS;

    current = getNextToken();
    error = checkToken(current);

    return (error);
}

// Init the scanner. and get the first token
int Scanner::init() {
    // Initialize all the class variables.
    unsigned int error = SUCCESS;
    num_literal_atom = 0;
    num_numeric_atom = 0;
    num_oepn_paran = 0;
    num_closing_paran = 0;
    store_flag = false;
    line_number = 1;
    char_count = 0;

    current = getNextToken();
    error = checkToken(current);

done:
    return (error);
}

// Print information about the tokens parsed
void Scanner::printTokens() {
    int total_sum = 0;
    // Prints the necessary information.
    cout << "LITERAL ATOMS: " << num_literal_atom;
    for (int i = 0; i < literal_atom_list.size(); i++) {
        cout <<  ", " << literal_atom_list[i];
    }
    for (int i = 0; i < numeric_atom_list.size(); i++) {
        total_sum += numeric_atom_list[i];
    }
    cout << "\nNUMERIC ATOMS: " << num_numeric_atom << ", "<< total_sum;
    cout << "\nOPEN PARENTHESES: " << num_oepn_paran;
    cout << "\nCLOSING PARENTHESES: " << num_closing_paran;
    cout << "\n";
}
