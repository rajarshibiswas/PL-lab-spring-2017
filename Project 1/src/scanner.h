/*
 * CSE 6341
 * Project: 1
 * File	  : scanner.h
 * Author : Rajarshi Biswas
 * Email  : biswas.91@osu.edu
 *
 */

#ifndef INTERPRETER_SCANNER_H
#define INTERPRETER_SCANNER_H

using namespace std;

// List of token types.
typedef enum token_type {
    LITERAL_ATOM,
    NUMERIC_ATOM,
    OPEN_PAREN,
    CLOSING_PAREN,
    ERROR,
    END_OF_FILE
} token_type;

// Different values that a token can take.
typedef struct token_value {
	string literal;
	int numeric;
	string error;
} token_value;

// The token structure.
typedef struct token {
	// type of the token
	token_type type;
	token_value value;
} token;

// The scanner API. Parses characters from the stdin and generates
// token. 
int scanner (void);

#endif
