/*
 * CSE 6341
 * Project: 3
 * File	  : eval.cpp
 * Author : Rajarshi Biswas
 * Email  : biswas.91@osu.edu
 *
 */

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "common.h"
#include "scanner.h"
#include "parser.h"
#include "eval.h"

static int length(tree *s) {
    //cout << "counnting len\n";
    if (s->val.type ==  NIL) {
        return 0;
    } else {
        return (1 + length(s->right));
    }
}

static tree* car(tree *s) {

    if (s->left == NULL) {
        return NULL;
    }
    return (s->left);

}

static tree* cdr(tree *s) {
     // TODO: Do I need to check both the nodes?
    if (s->right == NULL) {
            return NULL;
    }
    return (s->right);
}

static tree* cons(tree *s1, tree* s2) {
    tree *s = newNode();
    s->left = s1;
    s->right = s2;
    return s;
}

static tree* atom(tree *s1) {
        tree *s = newNode();
        if (s1->left == NULL && s1->right == NULL) {
            s->val.type = T;
        } else {
            s->val.type = NIL;
        }
        return s;
}

static tree* checkInt(tree *s1) {
    tree *temp = newNode();
    if (s1->val.type == NUMERIC_ATOM && s1->right == NULL && s1->left == NULL) {
        cout << "\nGot numerica Atom";
        temp->val.type = T;
        return temp;
    } else {
        temp->val.type = NIL;
        return temp;
    }
}

static tree* checkNull(tree *s1) {
    tree *s = newNode();
    if (s1->val.type == NIL && s1->right == NULL && s1->left == NULL) {
        cout << "\nGot numerica Atom";
        s->val.type = T;
        return s;
    } else {
        s->val.type = NIL;
        return s;
    }
}

static tree* eq(tree *s1, tree *s2) {
    tree *s = newNode();
/* TODO
    if ((singleNode(s1) != 0) ||
        (singleNode(s2) != 0)) {
        return NULL;
    }
    */

    if (s1->val.type == LITERAL_ATOM && s2->val.type == LITERAL_ATOM) {
        if (s1->val.value.literal.compare(s2->val.value.literal) == 0) {
            s->val.type = T;
        } else {
            s->val.type = NIL;
        }

    } else if (s1->val.type == NUMERIC_ATOM && s2->val.type == NUMERIC_ATOM) {
        if (s1->val.value.numeric == s2->val.value.numeric) {
            s->val.type = T;
        } else {
            s->val.type = NIL;
        }

    } else {
        s->val.type = NIL;

    }
    return s;
}

static tree* plus(tree *s1, tree *s2) {
    /* TODO
        if ((singleNode(s1) != 0) ||
            (singleNode(s2) != 0)) {
            return NULL;
        }
        */
        /* TODO
            if ((numericNode(s1) != 0) ||
                (numericNode(s2) != 0)) {
                return NULL;
            }
            */
    tree *s = newNode();

    s->val.type = NUMERIC_ATOM;
    s->value.numeric = s1->val.value.numeric + s2->val.value.numeric;
    return s;
}

static tree* minus(tree *s1, tree *s2) {
    /* TODO
        if ((singleNode(s1) != 0) ||
            (singleNode(s2) != 0)) {
            return NULL;
        }
        */
        /* TODO
            if ((numericNode(s1) != 0) ||
                (numericNode(s2) != 0)) {
                return NULL;
            }
            */
    tree *s = newNode();

    s->val.type = NUMERIC_ATOM;
    s->value.numeric = s1->val.value.numeric + s2->val.value.numeric;
    return s;
}


tree* eval(tree *parse_tree) {
    int len;
    tree *result = newNode();
    cout << "\nevaluating";
    // parse_tree should be a list.  Do I need to check ?
    tree *output, *s1, *s2, *temp, *carnode;
    string carnode_val;

    // Tree contains only one litteral atom T
    if (parse_tree->val.type == T) {
        output = parse_tree;
        goto end;
    }

    // contains only the literal atom NIL
    if (parse_tree->val.type == NIL) {
        output = parse_tree;
        goto end;
    }

    // Given a binary tree, we have only one numeric atom.
     temp = checkInt(parse_tree);
    if (temp->val.type != NIL) {
        output = parse_tree;
        cout << "sending back the numerica data" << output->val.value.numeric;
        goto end;
    }

    // Other Atoms eval is undefined.

    // Get the car node value.
    carnode = car(parse_tree);
    cout << "\nGetting the car\n";
    if (carnode == NULL) {
        output = NULL;
        goto end;

    }
    carnode_val = carnode->val.value.literal;

    // Cosume the op

    s1 = parse_tree->right->left;
    s2 = parse_tree->right->right->left;

    if (carnode_val.compare("PLUS") == 0) {
            len = length(parse_tree);
            s1 = eval(s1);
            s2 = eval(s2);
            if (len != 3 || s1 == NULL || s2 == NULL) {
                // An Error has happened.
                output = NULL;
                goto end;
            }
            cout << "\nvalue : " << s1->val.value.numeric;
            cout <<"\n"<< s2->val.value.numeric << "\n";
            result->val.value.numeric = s1->val.value.numeric + s2->val.value.numeric;
            output = result;
            goto end;
    } else if (carnode_val.compare("MINUS") == 0) {
        s1 = eval(s1);
        s2 = eval(s2);
        //cout << "\nvalue : " << s1->val.value.numeric;
        //cout <<"\n"<< s2->val.value.numeric << "\n";
        result->val.value.numeric = s1->val.value.numeric - s2->val.value.numeric;
        output = result;
        goto end;
    } else if (carnode_val.compare("TIMES") == 0) {
        s1 = eval(s1);
        s2 = eval(s2);
        //cout << "\nvalue : " << s1->val.value.numeric;
        //cout <<"\n"<< s2->val.value.numeric << "\n";
        result->val.value.numeric = s1->val.value.numeric * s2->val.value.numeric;
        output = result;
        goto end;
    }
        /*
        case MINUS:
        case TIMES:
        case LESS:
        case GREATER:
        */




end:
    return (output);
}
