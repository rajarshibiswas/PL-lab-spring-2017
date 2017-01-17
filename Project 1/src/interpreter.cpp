#include <iostream>
#include <cstdlib>
#include <vector>
#include "common.h"
#include "scanner.h"

int main (int argc, char *argv[]) {
    int error = SUCCESS;
    // Calls the scanner to parse the input and generates token.
    error = scanner();
    return error;
}
