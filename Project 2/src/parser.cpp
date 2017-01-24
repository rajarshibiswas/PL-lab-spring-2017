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

/*
static tree* newNode() {
    tree *node = new tree;
    node->left = NULL;
    node->right = NULL;

    return (node);
}
*/
static tree* parseExpr(Scanner *scan) {
    int error = SUCCESS;

    if (scan->getCurrent().type == LITERAL_ATOM ||
        scan->getCurrent().type == NUMERIC_ATOM ||
        scan->getCurrent().type == NIL) {

            // Create a node
            tree *node = new tree;
            node->val = scan->getCurrent();
            node->right = NULL;
            node->left = NULL;
            scan->moveToNext(); // Consume the atom

            return node;

    } else if (scan->getCurrent().type == OPEN_PAREN) {
        // List of expression. Creat an empty node
        tree *node = new tree;
        tree *h = node;
        node->val.type = NAN;
        node->right = NULL;
        node->left = NULL;

        scan->moveToNext();

        while ((scan->getCurrent()).type != CLOSING_PAREN) {
            if (node->val.type == NIL) {
                node->val.type = NAN;
            }

            node->left = parseExpr(scan);

            tree *temp = new tree;
            temp->left = NULL;
            temp->right = NULL;
            temp->val.type = NIL;

            node->right = temp;
            node = node->right;
        }

        // Special case for handling ()
        scan->moveToNext(); // Consume closing parenthesis.

        return h;
    } else {
        error = PARSING_ERROR;
        return NULL;
    }
}

void parse_tree_print(tree *cur_node) {
    if (cur_node == NULL) {
        cout << "NIL";
        return;
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
            cout << ". ";
            parse_tree_print(cur_node->right);
            cout << ")";
    }
    return;
}

int parseStart(Scanner *scan) {
    int error = SUCCESS;
    do {
        tree *parse_tree = new tree;
        // Pass an empty tree.
        parse_tree = parseExpr(scan);
        if (parse_tree == NULL) {
            break;
        }
        //clecout << "out of parseExpr\n";
        parse_tree_print(parse_tree);
        cout << "\n";
    } while (scan->getCurrent().type != END_OF_FILE);

    return (error);
}
