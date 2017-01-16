#include <iostream>
#include <cstdlib>
#include <vector>

#include "common.h"
#include "scanner.h"

int main (int argc, char *argv[]) {
    int error = SUCCESS;
    error = scanner();
    return error;
}
