/*
 * CSE 6341
 * Project: 4
 * File	  : parser.h
 * Author : Rajarshi Biswas
 * Email  : biswas.91@osu.edu
 *
 */

#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H

using namespace std;

typedef struct tree {
    token val;
    tree *right;
    tree *left;
} tree;

int parseStart(Scanner *scan);
//void parse_tree_print(tree *t);
tree* newNode();

#endif
