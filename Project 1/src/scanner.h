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

typedef struct token_value {
	string literal;
	int numeric;
	string error;
} token_value;

typedef struct token {
	// type of the token
	token_type type;
	token_value value;
} token;

int scanner (void);

#endif
