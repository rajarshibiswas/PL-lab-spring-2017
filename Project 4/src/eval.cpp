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
    if (singleNode(s)) {
        cout << "ERROR: CAR can't be done on a ATOM";
    }
    if (s->left == NULL) {
        return NULL;
    }
    return (s->left);

}

static tree* cdr(tree *s) {
    if (singleNode(s)) {
        cout << "ERROR: CDR can't be done on a ATOM";
    }
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
    tree *temp = newNode();
    if (s1->val.type == NUMERIC_ATOM && s1->right == NULL && s1->left == NULL) {
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
        s->val.type = T;
        return s;
    } else if (s1->val.type == LITERAL_ATOM && s1->val.value.literal.compare("NIL") == 0) {
        s->val.type = T;
        return s;
    }  else {
        s->val.type = NIL;
        return s;
    }
}

static tree* lisp_eq(tree *s1, tree *s2) {
    tree *s = newNode();
    if ((singleNode(s1) == 0) ||
        (singleNode(s2) == 0)) {
        cout << "ERROR: One of the arguments for EQ is not an ATOM.";
        return NULL;
    }

    // Check for literal atom
    if (s1->val.type == LITERAL_ATOM && s2->val.type == LITERAL_ATOM) {
        if (s1->val.value.literal.compare(s2->val.value.literal) == 0) {
            s->val.type = T;
        } else {
            s->val.type = NIL;
        }

    } else if (s1->val.type == NUMERIC_ATOM && s2->val.type == NUMERIC_ATOM) {
        // Check for numeric atom
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
        cout << "ERROR: One of the arguments for PLUS is not Numeric ATOM.";
        return NULL;
    }
    if ((numericNode(s1) == 0) || (numericNode(s2) == 0)) {
            cout << "ERROR: One of the arguments for PLUS is not Numeric ATOM.";
            return NULL;
    }
    tree *s = newNode();

    s->val.type = NUMERIC_ATOM;
    s->val.value.numeric = s1->val.value.numeric + s2->val.value.numeric;
    return s;
}

static tree* lisp_minus(tree *s1, tree *s2) {
    if ((singleNode(s1) == 0) || (singleNode(s2) == 0)) {
        cout << "ERROR: One of the arguments for MINUS is not Numeric ATOM.";
        return NULL;
    }
    if ((numericNode(s1) == 0) || (numericNode(s2) == 0)) {
        cout << "ERROR: One of the arguments for MINUS is not Numeric ATOM.";
        return NULL;
    }
    tree *s = newNode();

    s->val.type = NUMERIC_ATOM;
    s->val.value.numeric = s1->val.value.numeric - s2->val.value.numeric;
    return s;
}

static tree* lisp_times(tree *s1, tree *s2) {
    if ((singleNode(s1) == 0) || (singleNode(s2) == 0)) {
        cout << "ERROR: One of the arguments for TIMES is not Numeric ATOM.";
        return NULL;
    }
    if ((numericNode(s1) == 0) || (numericNode(s2) == 0)) {
        cout << "ERROR: One of the arguments for TIMES is not Numeric ATOM.";
        return NULL;
    }

    tree *s = newNode();
    s->val.type = NUMERIC_ATOM;
    s->val.value.numeric = s1->val.value.numeric * s2->val.value.numeric;
    return s;
}

static tree* lisp_less(tree *s1, tree *s2) {
    if ((singleNode(s1) == 0) || (singleNode(s2) == 0)) {
        cout << "ERROR: One of the arguments for LESS is not Numeric ATOM.";
        return NULL;
    }
    if ((numericNode(s1) == 0) || (numericNode(s2) == 0)) {
        cout << "ERROR: One of the arguments for LESS is not Numeric ATOM.";
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
        cout << "ERROR: One of the arguments for GREATER is not Numeric ATOM.";
        return NULL;
    }
    if ((numericNode(s1) == 0) || (numericNode(s2) == 0)) {
        cout << "ERROR: One of the arguments for GREATER is not Numeric ATOM.";
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

/*
static tree *lisp_cond(tree *s) {
    while (s->val.type != NIL) {
        tree *s1 = s->left;
        if (length(s1) != 2) {
            cout << "ERROR: COND should have two arguments.";
            return NULL;
        }
        tree *b = s1->left;
        tree *e = s1->right->left;
        tree *temp = eval(b);
        if (temp == NULL) {
            //cout << " Undefined expression while doing COND.";
            return NULL;
        }

        if (temp->val.type == NIL ||
            (temp->val.type == LITERAL_ATOM &&
            temp->val.value.literal.compare("NIL") == 0)) {
                // Check for NIL
                goto next;
        } else {
            temp = eval(e);
            if (temp == NULL) {
                //cout << " Undefined expression while doing COND.";
                return NULL;
            }
            return temp;
        }
next: // Get the next condition.
        s = s->right;
    }
    cout << "ERROR: Undefined expression while doing COND.";
    return (NULL);
}*/

static int bound(tree *x, tree *z) {
    tree *temp;
    temp = checkNull(z);
    if (temp->val.type == T) {
        free(temp);
        return (NIL);
    } else {
        temp = lisp_eq(x, z->left->left);
        if (temp->val.type == T) {
            free (temp);
            return (T);
        } else {
            free (temp);
            return bound(x, cdr(z));
        }
    }
}

static
tree* getval(tree *x, tree* z) {
    if (bound(x, z) == T) {
        tree *temp = lisp_eq(x, car(car(z)));
        if (temp->val.type == T) {
            free(temp);
            return (car(cdr(car(z))));
        } else {
            free(temp);
            return (getval(x, cdr(z)));
        }
    } else {
        // CONTROL SHOULD NOT COME HERE. DEAD CODE.
        tree *result_node = newNode();
        result_node->val.type = NIL;
        // return NIL node
        return (result_node);
    }
}

static
tree* addPairs(tree* x, tree* y, tree* z) {
    tree *temp;
    temp = checkNull(x);

    if (temp->val.type == T) { // NIL
        free(temp);
        return (z);
    } else {
        free(temp);
        temp = cons ( (cons( car(x) , car(y))),
                              (addPairs(cdr(x), cdr(y), z))
                    );
        return temp;
    }
}

// TODO type of a and d
static
tree *evcon(tree *x, tree* a, tree *d) {
    tree *temp = NULL;

    temp = checkNull(x);
    if (temp->val.type == T) {
        // ERROR
        free (temp);
        return (NULL);
    }
    free(temp);

    if (length(x->left) != 2) {
        cout << "ERROR: COND lists should have two arguments.";
        return NULL;
    }

    temp = eval(x->left->left /*CAAR*/, a, d);
    if (temp->val.type == NIL ||
        (temp->val.type == LITERAL_ATOM &&
        temp->val.value.literal.compare("NIL") == 0)) {
            // Check for NIL
            goto next;
    } else {
        return (eval(x->left->right->left /*CADAR*/, a , d));
    }
next:
    return (evcon(x->right /* CDR */, a, d));
}

static
tree* evlist(tree *x, tree *a, tree *d) {
    tree *temp = NULL;

    temp = checkNull(x);
    if (temp->val.type == T) {
        // ERROR
        //free (temp);
        temp->val.type = NIL;
        return (temp);
    }
    free(temp);
    tree *t1 = eval(x->left /*CAR*/, a, d);
    if (t1 == NULL) {
        return NULL;
    }
    tree *t2 = evlist(x->right /*CDR*/, a, d);
    if (t2 == NULL) {
        return NULL;
    }

    temp = cons(t1, t2);
    return (temp);

}


static
tree* apply(tree *f, tree *x, tree* a, tree* d) {

    tree *output = NULL;
    int fun_num = 0;
    tree *s1, *s2;

    if (singleNode(f) == 1) { // Yes its an Atom

        // First check the predefined fun list.
        try {
            cout << "DEBUG: " << __LINE__ << " - GOT Function" << f->val.value.literal << "\n";
            fun_num = function_list.at(f->val.value.literal);
        } catch (std::exception& e) {
            // Could it be a function in the d list?
            if (bound(f, d) == NIL) {
                // Oops! not there in d list too.
                output = NULL;
                cout << "ERROR: FUNCTION '"<< f->val.value.literal << "' not supported.";
                return (output);
            }
        }

        // SOME hack just for making my life simple
        tree *parse_tree = newNode();
        parse_tree->left = f; // Function name
        parse_tree->right = x; // Param list

        int len = length(parse_tree);

        switch (fun_num) {
            case 0: // PLUS
                if (len != 3) {
                    output = NULL;
                    cout << "ERROR: PLUS should have two arguments.";
                    goto end;
                }
                s1 = parse_tree->right->left;
                s2 = parse_tree->right->right->left;
                output = lisp_plus(s1, s2);
                goto end;
                break;
            case 1: // MINUS
                if (len != 3) {
                    output = NULL;
                    cout << "ERROR: MINUS should have two arguments.";
                    goto end;
                }
                s1 = parse_tree->right->left;
                s2 = parse_tree->right->right->left;
                output = lisp_minus(s1, s2);
                goto end;
                break;
            case 2: // TIMES
                if (len != 3) {
                    output = NULL;
                    cout << "ERROR: TIMES should have two arguments.";
                    goto end;
                }
                s1 = parse_tree->right->left;
                s2 = parse_tree->right->right->left;
                output = lisp_times(s1, s2);
                goto end;
                break;
            case 3: // LESS
                if (len != 3) {
                    output = NULL;
                    cout << "ERROR: LESS should have two arguments.";
                    goto end;
                }
                s1 = parse_tree->right->left;
                s2 = parse_tree->right->right->left;
                output = lisp_less(s1, s2);
                goto end;
                break;
            case 4: // GREATER
                if (len != 3) {
                    output = NULL;
                    cout << "ERROR: GREATER should have two arguments.";
                    goto end;
                }
                s1 = parse_tree->right->left;
                s2 = parse_tree->right->right->left;
                output = lisp_greater(s1, s2);
                goto end;
                break;
            case 5: // EQ
                if (len != 3) {
                    cout << "ERROR: EQ should have two arguments.";
                    output = NULL;
                    goto end;
                }
                s1 = parse_tree->right->left;
                s2 = parse_tree->right->right->left;
                output = lisp_eq(s1, s2);
                goto end;
                break;
            case 6: // Atom
                if (len != 2) {
                    cout << "ERROR: ATOM should have one argument.";
                    output = NULL;
                    goto end;
                }
                s1 = parse_tree->right->left;
                output = lisp_atom(s1);
                goto end;
                break;
            case 7:// INT
                if (len != 2) {
                    cout << "ERROR: INT should have one argument.";
                    output = NULL;
                    goto end;
                }
                s1 = parse_tree->right->left;
                output = checkInt(s1);
                goto end;
                break;
            case 8: // NULL
                if (len != 2) {
                    cout << "ERROR: NULL should have one argument.";
                    output = NULL;
                    goto end;
                }
                s1 = parse_tree->right->left;
                output = checkNull(s1);
                goto end;
                break;
            case 9: // CAR
                if (len != 2) {
                    cout << "ERROR: CAR should have one argument.";
                    output = NULL;
                    goto end;
                }
                s1 = parse_tree->right->left;
                if (s1->val.type == LITERAL_ATOM || s1->val.type == NUMERIC_ATOM) {
                    cout << "ERROR: Evaluation failed as CAR argument is atom.";
                }
                output = car(s1);
                goto end;
                break;
            case 10: // CDR
                if (len != 2) {
                    output = NULL;
                    cout << "ERROR: CDR should have one argument.";
                    goto end;
                }
                s1 = parse_tree->right->left;
                output = cdr(s1);
                goto end;
                break;
            case 11: // CONS
               if (len != 3) {
                   cout << "ERROR: CONS should have two arguments.";
                    output = NULL;
                    goto end;
                }
                s1 = parse_tree->right->left;
                s2 = parse_tree->right->right->left;
                output = cons(s1, s2);
                goto end;
                break;
            default:
                // Call Dlist function
                // TODO Check if formals and actual matches.
                output = eval( (cdr ( getval(f,d) ) ),
                               (addPairs ((car (getval (f,d))), x, a)),
                               d );
                goto end;
                break;
        }
    } else {
        cout << "ERROR: Undefined expression. Problem with LISP code.";
    }
end:
    return (output);

}


tree*
eval(tree *parse_tree, tree *a, tree *d) {
    int len;
    int fun_num;
    tree *output, *s1, *s2, *temp, *carnode;
    string carnode_val;

    // Its an ATOM
    if (singleNode(parse_tree)) {
            // Is it equal to T ?
            if ((parse_tree->val.type == T) ||
                 (parse_tree->val.type == LITERAL_ATOM &&
                  parse_tree->val.value.literal.compare("T") == 0)) {
                      // Return T
                      output = parse_tree;
                      return output;
            }

            // Is it equal to NIL ?
            if ((parse_tree->val.type == NIL)    ||
                 (parse_tree->val.type == EMPTY) ||
                 (parse_tree->val.type == LITERAL_ATOM &&
                  parse_tree->val.value.literal.compare("NIL") == 0)) {
                      parse_tree->val.type = NIL;
                      // Return NIL
                      output = parse_tree;
                      return output;
            }
            // Is it equal to Int ?
            temp = checkInt(parse_tree);
            if (temp->val.type != NIL) { // Yes indeed
                output = parse_tree;
                free(temp);
                return (output);
            }

            // Maybe a single value that was bound somewhere in the a-list?
            if (bound(parse_tree, a) == T) {
                return (getval(parse_tree, a));
            }

            // ERROR;
            //free(parse_tree);
            cout << "ERROR: Undefined!\n";
            return (NULL);
    }

    // Otherwise must be a list. Do some sanity check
    len = length(parse_tree);
    if (len < 2) {
        output = NULL;
        cout << "ERROR: The S expression is not supported.";
        goto end;
    }

    // Get the car node value.
    carnode = car(parse_tree);
    if (carnode == NULL) {
        output = NULL;
        goto end;
    }

    carnode_val = carnode->val.value.literal;
    cout << "DEBUG : function name: " << carnode_val << "\n";

    if (carnode_val.compare("QUOTE") == 0){
        if (len != 2) {
            cout << "ERROR: QUOTE should have one argument.";
            output = NULL;
            goto end;
        }
        // CADR
        s1 = parse_tree->right->left;
        output = s1;
        return output;
    } else if (carnode_val.compare("COND") == 0) {
        if (len < 2) {
            cout << "ERROR: COND should have atleast one argument.";
            output = NULL;
            goto end;
        }
        output = evcon(parse_tree->right, a, d);
        return output;
    } else if (carnode_val.compare("DEFUN") == 0) {
        // TODO Add stuff to D list
        d = cons(parse_tree->right->left, d);
        if (len != 4) {
            cout << "ERROR: DEFUN should have two arguments.";
            output = NULL;
            goto end;
        }
        output = newNode();
        output->val.type = USER_DEF_FUN;
        output->val.value.literal = parse_tree->right->left->val.value.literal;
        return (output);  // TODO: What to return from here?
    } else {
        cout << "Debug : Calling apply\n";
        // CALL the function with the param list.
        output = apply(car(parse_tree),
                       evlist(cdr(parse_tree), a, d),
                       a, d);
        if (output == NULL) {
            // TODO : some problem need to clean up.
            // free (output);
        }
        return (output);
    }

end:
    return (output);
}

/*
* Init the function mapping.
*/
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
    function_list["DEFUN"] = 14;

    // Init the D list.
    return SUCCESS;
}

/*
 * Print the Eval tree.
*/
void print_eval_tree(tree *s) {
    if (s == NULL) {
        // Just safe check.
        return;
    }

    if (singleNode(s) == 1) { // Leaf node
        if (s->val.type == NUMERIC_ATOM) {
            cout << s->val.value.numeric;
        } else if (s->val.type == T) {
            cout << "T";
        } else if (s->val.type == LITERAL_ATOM) {
            cout << s->val.value.literal;
        } else if (s->val.type == USER_DEF_FUN) {
            cout << s->val.value.literal;
        } else {
            cout << "NIL";
        }
        return;
    } else {
        cout << "(";
        while (singleNode(s) != 1) {
            print_eval_tree(s->left);
            if (singleNode(s->right) != 1) {
                //If K < n print a space
                cout << " ";
            }
            s = s->right;
        }
        if (s->val.type == NIL || (s->val.type == LITERAL_ATOM
            && s->val.value.literal.compare("NIL") == 0)) {
                cout << ")";
            } else {
                cout << " . ";
                if (s->val.type == NUMERIC_ATOM) {
                    cout << s->val.value.numeric;
                } else if (s->val.type == T) {
                    cout << "T";
                } else if (s->val.type == LITERAL_ATOM) {
                    cout << s->val.value.literal;
                }
                cout << ")";
            }
    }
    return;
}
