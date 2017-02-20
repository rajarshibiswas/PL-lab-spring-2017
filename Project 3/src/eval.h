/*
 * CSE 6341
 * Project: 3
 * File	  : eval.cpp
 * Author : Rajarshi Biswas
 * Email  : biswas.91@osu.edu
 *
 */

 #ifndef INTERPRETER_EVAL_H
 #define INTERPRETER_EVAL_H

using namespace std;

tree* eval(tree *parse_tree);
int init_eval();
void print_eval_tree(tree* s);
 #endif
