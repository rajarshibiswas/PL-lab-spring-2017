/*
 * CSE 6341
 * Project: 2
 * File	  : interpreter.cpp
 * Author : Rajarshi Biswas
 * Email  : biswas.91@osu.edu
 *
 */

#include <iostream>
#include <cstdlib>
#include <vector>
#include "common.h"
#include "scanner.h"
#include "parser.h"

int main (int argc, char *argv[]) {
    // Make a scanner object to parset the input.
    Scanner scan;
    int error = SUCCESS;
    // Calls the scanner to parse the input and generates token.
    error = scan.init();
    parseStart(&scan);

    cout << "\n";
    // print the tokens
    scan.printTokens();
    return error;
}
