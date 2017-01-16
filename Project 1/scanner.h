#ifndef INTERPRETER_SCANNER_H
#define INTERPRETER_SCANNER_H

using namespace std;

// List of tokens
typedef enum {
    LITERAL_ATOM,
    NUMERIC_ATOM,
    OPEN_PAREN,
    CLOSING_PAREN,
    ERROR,
    END_OF_FILE
} token;

int scanner (void);

#endif
