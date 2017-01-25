/*
 * CSE 6341
 * Project: 2
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
    END_OF_FILE,
    NIL,
    EMPTY // No value
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

// The scanner class.
class Scanner {

    private:
        // Used to store a unparsed char.
        bool store_flag;
        char store_char;
        // Used to store all the atom values.
        vector<string> literal_atom_list;
        vector<int> numeric_atom_list;
        unsigned int num_literal_atom;
        unsigned int num_numeric_atom;
        unsigned int num_oepn_paran;
        unsigned int num_closing_paran;
        token current;

        bool end_of_token(char input);
        int checkToken(token t);
        token getNextToken();
        char scanGetChar();

    public:
        char global_char;
        int line_number;
        int char_count;
        int init();
        int moveToNext();
        token getCurrent();
        void printTokens();

};

#endif
