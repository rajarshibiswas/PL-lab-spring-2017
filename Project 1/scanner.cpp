#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include "common.h"
#include "scanner.h"

using namespace std;

// Used to store a unparsed char. 
static bool store_flag;
static char store_char;

// Used to store all the atom values.
static vector<string> literal_atom_list;
static vector<int> numeric_atom_list;

// Stores the error token
string error_token;

static token getNextToken () {
    token t;
    char input;

    if (store_flag == true) {
        input = store_char;
    } else {
        input = getchar();
    }
    store_flag = false;
    
//    printf("\ninput - %d", input);
    if (input == EOF) {
        t =  END_OF_FILE;
        goto end;
    }

    // Consume all the white space, carriage return and line feed
    while (input == 32 ||  input == 13 || input == 10) {
        input = getchar();
    }
    
    if (input == EOF) {
        t =  END_OF_FILE;
        goto end;
    } else if (input == '(') {
        t =  OPEN_PAREN;
        goto end;
    } else if (input == ')') {
        t = CLOSING_PAREN;
        goto end;
    } else if (isupper(input)) {
        string literal_atom = "";
        literal_atom += input;
        input = getchar();
        while (isupper(input) || isdigit(input)) {
            literal_atom = literal_atom + input;
            input = getchar();
        }
        if (input != 32 && input != 13 && input != 10 && input != '(' && input !=')') {
            literal_atom += input;
            input = getchar();
            while (input != 32 && input != 13 && input != 10 && input != '(' && input !=')') {
                literal_atom += input;
                input = getchar();
            }
            error_token = literal_atom;
            t =  ERROR;
            goto end;
        }
        store_char = input;
        store_flag = true;

        literal_atom_list.push_back(literal_atom);
        t = LITERAL_ATOM;
        goto end;
    } else if (isdigit(input)) {
        string numeric_atom = "";
        numeric_atom += input;
        input = getchar();
        while (isdigit(input)) {
            numeric_atom += input;
            input = getchar();
        }
        if (input != 32 && input != 13 && input != 10 && input != '(' && input !=')') {
            numeric_atom += input;
            input = getchar();
            while (input != 32 && input != 13 && input != 10 && input != '(' && input !=')') {
                numeric_atom += input;
                input = getchar();
            }
            error_token = numeric_atom;
            t = ERROR;
            goto end;
        }
        store_char = input;
        store_flag = true;

        numeric_atom_list.push_back(stoi(numeric_atom));
        t = NUMERIC_ATOM;
        goto end;
    }

end:
    return t;
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
        switch (t) {
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
                 cout << "\nERROR: Invalid token " << error_token;
                 goto out;
                 break;
        }
    }

done: 
    cout << "LITERAL ATOMS: " << num_literal_atom;
    for (int i = 0; i < literal_atom_list.size(); i++) {
        cout <<  ", " << literal_atom_list[i]; 
    }

    for (int i = 0; i < numeric_atom_list.size(); i++) {
        total_sum += numeric_atom_list[i];
    }
    cout << "\nNUMERIC ATOMS: " << num_numeric_atom << ", " << total_sum;

    cout << "\nOPEN PARENTHESES: " << num_oepn_paran;
    cout << "\nCLOSING PARENTHESES: " << num_closing_paran;

out:
    cout << "\n";
    
    return (error);
}
