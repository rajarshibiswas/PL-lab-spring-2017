/*
 * CSE 6341
 * Project: 2
 * File	  : parser.cpp
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

//token EMPTY_NODE = {NAN};
//token NIL_NODE = {NIL};

static tree* parseExpr(Scanner *scan, tree *cur_node) {
    int error = SUCCESS;

    if (scan->getCurrent().type == LITERAL_ATOM ||
        scan->getCurrent().type == NUMERIC_ATOM ||
        scan->getCurrent().type == NIL) {

            // Create a node
            tree *node = new tree;
            node->val = scan->getCurrent();
            node->right = NULL;
            node->left = NULL;
            cout << "creating node " << scan->getCurrent().type << "\n";
            scan->moveToNext(); // Consume the atom

            if (cur_node == NULL) {
                // Empty tree. This is the first node.
                cur_node =  node;
                return cur_node;
            } else {
                // Non empty tree
                cur_node->left = node; // Atom goes in the left
                // No need to change current node
                return cur_node;
            }

    } else if (scan->getCurrent().type == OPEN_PAREN) {
        cout << "encounterint open paran\n";
        // List of expression. Creat an empty node
        tree *node = new tree;
        node->val.type = NAN;
        node->right = NULL;
        node->left = NULL;

        scan->moveToNext();

        if (cur_node == NULL) {
            // No root this is the first. Make it root.
            cur_node = node;
        } else {
            // One more parenthesis
            cur_node->left = node; // Hang the empty tree in the left side.
        //    tree *temp = cur_node;

        //    cur_node = node;
            parseExpr(scan, node);



        //    return cur_node;

/*
            tree *temp = new tree;
            temp->left = NULL;
            temp->right = NULL;
            temp->val.type = NIL;
            cur_node->right = temp;
            //cur_node = cur_node->right;
            //return cur_node;*/
        }

        if ((scan->getCurrent()).type != CLOSING_PAREN) {
            while ((scan->getCurrent()).type != CLOSING_PAREN) {
                if (cur_node->val.type == NIL) {
                    // Change it to EMPTY node as expression left.
                    cur_node->val.type = NAN;
                }
                cur_node = parseExpr(scan, cur_node);
                tree *temp = new tree;
                temp->left = NULL;
                temp->right = NULL;
                temp->val.type = NIL;
                cur_node->right = temp;
                cur_node = cur_node->right;
            }
        } else {
            tree *temp = new tree;
            temp->left = NULL;
            temp->right = NULL;
            temp->val.type = NIL;
            cur_node->right = temp;
        }
        // Special case for handling ()
        scan->moveToNext(); // Consume closing parenthesis.

        return node;
    } else {
        error = PARSING_ERROR;
        return NULL;
    }
}

void parse_tree_print(tree *cur_node) {


    if (cur_node == NULL) {
        cout << "NIL";

    } else if (cur_node->left == NULL && cur_node->right == NULL) {
        // Atom node
        if (cur_node->val.type == LITERAL_ATOM) {
            cout << cur_node->val.value.literal;
        } else if (cur_node->val.type == NUMERIC_ATOM) {
            cout << cur_node->val.value.numeric;
        } else {
            cout << "NIL";
        }
        return;
    } else {
            cout << "(";
            parse_tree_print(cur_node->left);
            cout << ".";
            parse_tree_print(cur_node->right);
            cout << ")";
    }
    return;
}

int parseStart(Scanner *scan) {
    int error = SUCCESS;

    //do {
        tree *parse_tree = new tree;
        cout << "Parsing\n";
        cout << (scan->getCurrent()).type;
        // Pass an empty tree.
        parse_tree = parseExpr(scan, NULL);
        cout << "out of parseExpr\n";
        parse_tree_print(parse_tree);
        cout << "\n\n";
    //} while (scan->getCurrent().type != END_OF_FILE);

    return (error);
}
