/*
 * CSE 6341
 * Project: 2
 * File	  : parser.h
 * Author : Rajarshi Biswas
 * Email  : biswas.91@osu.edu
 *
 */

#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H

using namespace std;

int parseStart(Scanner *scan);
typedef struct tree {
    token val;
    tree *right;
    tree *left;
} tree;


#endif
