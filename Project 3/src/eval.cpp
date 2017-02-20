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
#include <map>

std::map<string,int> function_list;


static int singleNode(tree *s) {

    if (s == NULL)
        return 0;
    if (s->left == NULL && s->right == NULL) {
        return 1;
    } else {
        return 0;
    }
}

static int numericNode(tree *s) {
    if (s->val.type == NUMERIC_ATOM) {
        return 1;
    } else {
        return 0;
    }
}


static int length(tree *s) {
    cout << "counnting len\n";
    if (singleNode(s)) {
        if (s->val.type ==  NIL) {
            return 0;
        } else {
            // TODO change it to min INT negative
            return (-9999);
        }
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
    cout << "I am consting";
    tree *s = newNode();
    s->left = s1;
    s->right = s2;
    if (singleNode (s2) == 1)
        cout << "\nm single \n";
    if (singleNode (s1) == 1)
        cout << "\nm single \n";
    return s;
}

static tree* lisp_atom(tree *s1) {
        tree *s = newNode();
        if (s1->left == NULL && s1->right == NULL) {
            s->val.type = T;
        } else {
            s->val.type = NIL;
        }
        return s;
}

static tree* checkInt(tree *s1) {
    //cout << "Checking int\n";
    tree *temp = newNode();
    if (s1->val.type == NUMERIC_ATOM && s1->right == NULL && s1->left == NULL) {
        //cout << "\nGot numerica Atom";
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
        //cout << "\nGot numerica Atom";
        s->val.type = T;
        return s;
    } else {
        s->val.type = NIL;
        return s;
    }
}

static tree* eq(tree *s1, tree *s2) {
    tree *s = newNode();

    if ((singleNode(s1) != 0) ||
        (singleNode(s2) != 0)) {
        return NULL;
    }

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

static tree* lisp_plus(tree *s1, tree *s2) {
    if (singleNode(s1) == 0 || singleNode(s1) == 0) {
        return NULL;
    }
    if ((numericNode(s1) == 0) || (numericNode(s2) == 0)) {
            return NULL;
    }
    tree *s = newNode();

    s->val.type = NUMERIC_ATOM;
    s->val.value.numeric = s1->val.value.numeric + s2->val.value.numeric;
    return s;
}

static tree* lisp_minus(tree *s1, tree *s2) {
    if ((singleNode(s1) == 0) || (singleNode(s2) == 0)) {
        return NULL;
    }
    if ((numericNode(s1) == 0) || (numericNode(s2) == 0)) {
        return NULL;
    }
    tree *s = newNode();

    s->val.type = NUMERIC_ATOM;
    s->val.value.numeric = s1->val.value.numeric - s2->val.value.numeric;
    return s;
}

static tree* lisp_times(tree *s1, tree *s2) {
    if ((singleNode(s1) == 0) || (singleNode(s2) == 0)) {
        return NULL;
    }
    if ((numericNode(s1) == 0) || (numericNode(s2) == 0)) {
        return NULL;
    }

    tree *s = newNode();
    s->val.type = NUMERIC_ATOM;
    s->val.value.numeric = s1->val.value.numeric * s2->val.value.numeric;
    return s;
}

static tree *lisp_cond(tree *s) {
    while (s->val.type != NIL) {
        tree *s1 = s->left;
        if (length(s1) != 2) {
            return NULL;
        }
        tree *b = s1->left;
        tree *e = s1->right->left;
        tree *temp = eval(b);
        if (temp == NULL) {
            // b undefined
            return NULL;
        }
        if (temp->val.type != NIL) {
            temp = eval(e);
            if (temp == NULL) {
                // n undefined
                return NULL;
            }
            return temp;
        }
        s = s->right;
    }
    return (NULL);
}

static tree* lisp_less(tree *s1, tree *s2) {
    if ((singleNode(s1) == 0) || (singleNode(s2) == 0)) {
        return NULL;
    }
    if ((numericNode(s1) == 0) || (numericNode(s2) == 0)) {
        return NULL;
    }

    tree *s = newNode();
    if (s1->val.value.numeric < s2->val.value.numeric) {
        s->val.type = T;
    } else {
        s->val.type = NIL;
    }
    return s;
}

static tree* lisp_greater(tree *s1, tree *s2) {
    if ((singleNode(s1) == 0) || (singleNode(s2) == 0)) {
        return NULL;
    }
    if ((numericNode(s1) == 0) || (numericNode(s2) == 0)) {
        return NULL;
    }

    tree *s = newNode();
    if (s1->val.value.numeric > s2->val.value.numeric) {
        s->val.type = T;
    } else {
        s->val.type = NIL;
    }
    return s;
}


tree* eval(tree *parse_tree) {
    int len;
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
        free(temp);
        goto end;
    }

    // Other Atoms eval is undefined.

    // Get the car node value.
    carnode = car(parse_tree);
    if (carnode == NULL) {
        output = NULL;
        goto end;
    }
    carnode_val = carnode->val.value.literal;
    len = length(parse_tree);

    
    switch (function_list[carnode_val]) {
        case 0: // PLUS
            if (len != 3) {
                output = NULL;
                goto end;
            }
            s1 = parse_tree->right->left;
            s2 = parse_tree->right->right->left;
            s1 = eval(s1);
            s2 = eval(s2);
            output = lisp_plus(s1, s2);
            free(s1);
            free(s2);
            goto end;
            break;
        case 1: // MINUS
            if (len != 3) {
                output = NULL;
                goto end;
            }
            s1 = parse_tree->right->left;
            s2 = parse_tree->right->right->left;
            s1 = eval(s1);
            s2 = eval(s2);
            output = lisp_minus(s1, s2);
            free(s1);
            free(s2);
            goto end;
            break;
        case 2: // TIMES
            if (len != 3) {
                output = NULL;
                goto end;
            }
            s1 = parse_tree->right->left;
            s2 = parse_tree->right->right->left;
            s1 = eval(s1);
            s2 = eval(s2);
            output = lisp_times(s1, s2);
            free(s1);
            free(s2);
            goto end;
            break;
        case 3: // LESS
            if (len != 3) {
                output = NULL;
                goto end;
            }
            s1 = parse_tree->right->left;
            s2 = parse_tree->right->right->left;
            s1 = eval(s1);
            s2 = eval(s2);
            output =  lisp_less(s1, s2);
            free(s1);
            free(s2);
            goto end;
            break;
        case 4: // GREATER
            if (len != 3) {
                output = NULL;
                goto end;
            }
            s1 = eval(s1);
            s2 = eval(s2);
            output =  lisp_greater(s1, s2);
            free(s1);
            free(s2);
            goto end;
            break;
        case 5: // EQ
            if (len != 3) {
                output = NULL;
                goto end;
            }
            s1 = parse_tree->right->left;
            s2 = parse_tree->right->right->left;
            s1 = eval(s1);
            s2 = eval(s2);
            output = eq(s1, s2);
            free(s1);
            free(s2);
            goto end;
            break;
        case 6: // Atom
            if (len != 2) {
                output = NULL;
                goto end;
            }
            s1 = parse_tree->right->left;
            s1 = eval(s1);
            output =  lisp_atom(s1);
            free(s1);
            goto end;
            break;
        case 7:// INT
            if (len != 2) {
                output = NULL;
                goto end;
            }
            s1 = parse_tree->right->left;
            s1 = eval(s1);
            output =  checkInt(s1);
            free(s1);
            goto end;
            break;
        case 8: // NULL
            if (len != 2) {
                output = NULL;
                goto end;
            }
            s1 = parse_tree->right->left;
            s1 = eval(s1);
            output =  checkNull(s1);
            free(s1);
            goto end;
            break;
        case 9: // CAR
            if (len != 2) {
                output = NULL;
                goto end;
            }
            s1 = parse_tree->right->left;
            s1 = eval(s1);
            output =  car(s1);
            free(s1);
            goto end;
            break;
        case 10: // CDR
            if (len != 2) {
                output = NULL;
                goto end;
            }
            s1 = parse_tree->right->left;
            s1 = eval(s1);
            output = cdr(s1);
            free(s1);
            goto end;
            break;
        case 11: // CONS
            cout << "I am suppost";
           if (len != 3) {
                output = NULL;
                goto end;
            }
            s1 = parse_tree->right->left;
            s2 = parse_tree->right->right->left;
            cout << "\nSingle ? " << singleNode(s1);
            s1 = eval(s1);
            s2 = eval(s2);
            cout << "\nSingle ? " << singleNode(s1);
            output = cons(s1, s2);
        //    free(s1);
        //    free(s2);
            goto end;
            break;
        case 12: // QUOTE
            if (len != 2) {
                output = NULL;
                goto end;
            }
            s1 = parse_tree->right->left;
            output = s1;
            goto end;
            break;
        case 13: // COND
            output = lisp_cond(parse_tree->right);
            goto end;
            break;

        default:
            output = NULL;
            break;
    }

end:
    return (output);
}

int init_eval() {
    function_list["PLUS"] = 0;
    function_list["MINUS"] = 1;
    function_list["TIMES"] = 2;
    function_list["LESS"] = 3;
    function_list["GREATER"] = 4;
    function_list["EQ"] = 5;
    function_list["ATOM"] = 6;
    function_list["INT"] = 7;
    function_list["NULL"] = 8;
    function_list["CAR"] = 9;
    function_list["CDR"] = 10;
    function_list["CONS"] = 11;
    function_list["QUOTE"] = 12;
    function_list["COND"] = 13;
    return SUCCESS;
}

void print_eval_tree(tree *s) {

    if (s == NULL)
        return;
    if (singleNode(s) == 1) {
        if (s->val.type == NUMERIC_ATOM) {
            cout << s->val.value.numeric <<" ";
        } else if (s->val.type == T) {
            cout << "T ";
        } else if (s->val.type == LITERAL_ATOM) {
            cout << s->val.value.literal << " ";
        } else {
            cout << "NIL ";
        }
        return;
    } else {
        cout << "(";
        while (singleNode(s) != 1) {
            print_eval_tree(s->left);
            s = s->right;
        }
            if (s->val.type == NIL) {
                cout << ")";
            } else {
                cout << " . ";
                if (s->val.type == NUMERIC_ATOM) {
                    cout << s->val.value.numeric;
                } else if (s->val.type == T) {
                    cout << "T ";
                } else if (s->val.type == LITERAL_ATOM) {
                    cout << s->val.value.literal;
                }
                cout << ")";
            }
    }
    return;
}
