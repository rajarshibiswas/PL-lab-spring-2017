/*
 * CSE 6341
 * Project: 4
 * File	  : eval.cpp
 * Author : Rajarshi Biswas
 * Email  : biswas.91@osu.edu
 *
 */

 #ifndef INTERPRETER_EVAL_H
 #define INTERPRETER_EVAL_H

using namespace std;

tree* eval(tree *parse_tree, tree *a);
int init_eval();
void print_eval_tree(tree* s);
extern tree *d;
 #endif
