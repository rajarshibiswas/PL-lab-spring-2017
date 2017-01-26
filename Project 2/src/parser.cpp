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

using namespace std;

/*
 * Create an empty node for the parse tree.
 * Returns a new new node.
 */
static tree* newNode() {
    tree *node = new tree;
    node->left = NULL;
    node->right = NULL;

    return (node);
}

/*
 * Parses the expression.
 * scan - The current scanner
 * Returns the head node of the parsed expression.
 */
static tree* parseExpr(Scanner *scan) {
    int error = SUCCESS;

    if (scan->getCurrent().type == LITERAL_ATOM ||
        scan->getCurrent().type == NUMERIC_ATOM ||
        scan->getCurrent().type == NIL) {

            // Create a node for the leaf nodes
            tree *node = newNode();
            node->val = scan->getCurrent();

            scan->moveToNext(); // Consume the atom

            return node;

    } else if (scan->getCurrent().type == OPEN_PAREN) {
        // List of expression. Creat an empty node
        tree *node = newNode();
        node->val.type = EMPTY;

        // Keep track of the current node, so that we can
        // return it to the caller.
        tree *head = node;

        scan->moveToNext(); // Consume the Open parenthesis
        if (scan->getCurrent().type == ERROR) {
            // If an error occured after an open parenthesis
            // i.e. the remaining expression is wrong.
            goto error;
        }

        while ((scan->getCurrent()).type != CLOSING_PAREN) {
            // Recursively call till we find a closeing parenthesis.
            if (node->val.type == NIL) {
                // This node will be used, so change the value
                node->val.type = EMPTY;
            }
            // Hang any found node in the left side.
            tree *left_side_tree_root = parseExpr(scan);
            if (left_side_tree_root == NULL) {
                // Some error has occured
                goto error;
            }
            node->left = left_side_tree_root;

            tree *temp = newNode();
            temp->val.type = NIL;
            node->right = temp;
            node = node->right;
        }
        scan->moveToNext(); // Consume closing parenthesis.
        return head;
    } else {
        goto error;
    }
error:
    return (NULL);
}

/*
 * Prints the parse tree.
 * cur_node - The root node of the parse tree.
 */
static void parse_tree_print(tree *cur_node) {
    if (cur_node == NULL) {
        // Just a safecheck.
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
            cout << " . ";
            parse_tree_print(cur_node->right);
            cout << ")";
    }
    return;
}

/*
 * Prints information about the parse error.
 * scan - The current scanner.
 */
static void parseError(Scanner *scan) {
    cout << "ERROR:";
    if (scan->getCurrent().type == ERROR) {
        cout << " Synatx error while parsing invalid token '" << scan->getCurrent().value.error<<"'";
        scan->char_count = scan->char_count - scan->getCurrent().value.error.length();
    } else {
        cout << " Synatx error while parsing token '";
        if (scan->global_char == EOF) {
            cout << "EOF'";
        } else {
            cout << scan->global_char << "'" ;
        }

    }
    //cout << "\nAt line number: " << scan->line_number;
    //cout << ", at starting char number: " << scan->char_count;
    cout << "\n";
}

/*
 * Starts the parsing and builds a parse tree.
 * scan - The current scanner.
 * Returns SUCCESS in case of success, otherwise an error.
 */
int parseStart(Scanner *scan) {
    int error = SUCCESS;
    do {
        tree *parse_tree;
        parse_tree = parseExpr(scan);
        if (parse_tree == NULL) {
            // Parsing error happened.
            parseError(scan);
            break;
        }
        //clecout << "out of parseExpr\n";
        parse_tree_print(parse_tree);
        cout << "\n";
    } while (scan->getCurrent().type != END_OF_FILE);

    return (error);
}
