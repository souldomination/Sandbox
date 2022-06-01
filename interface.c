#include <stdio.h>
#include <stdlib.h>

struct node {
    int (*eval)(const void *);
    const void *aux;
};

struct add {
    struct node node;
    struct node *left, *right;
};

struct sub {
    struct node node;
    struct node *left, *right;
};

struct lit {
    struct node node;
    int x;
};

int node_eval(struct node *node) {
    return node->eval(node->aux);
}

int add_eval(const void *aux) {
    const struct add *add = aux;
    return node_eval(add->left) + node_eval(add->right);
}

int sub_eval(const void *aux) {
    const struct sub *sub = aux;
    return node_eval(sub->left) - node_eval(sub->right);
}

int lit_eval(const void *aux) {
    const struct lit *lit = aux;
    return lit->x;
}

struct node *add_new(struct node *left, struct node *right) {
    struct add *add = malloc(sizeof(*add));
    add->node.eval = add_eval;
    add->node.aux = add;
    add->left = left;
    add->right = right;
    return &add->node;
}

struct node *sub_new(struct node *left, struct node *right) {
    struct sub *sub = malloc(sizeof(*sub));
    sub->node.eval = sub_eval;
    sub->node.aux = sub;
    sub->left = left;
    sub->right = right;
    return &sub->node;
}

struct node *lit_new(int x) {
    struct lit *lit = malloc(sizeof(*lit));
    lit->node.eval = lit_eval;
    lit->node.aux = lit;
    lit->x = x;
    return &lit->node;
}

int main(void) {
    /* Construct a tree representing '((5 - (2 + 1)) + (4 - 3))'. */
    struct node *tree = add_new(sub_new(lit_new(5),
                                        add_new(lit_new(2),
                                                lit_new(1))),
                                sub_new(lit_new(4),
                                        lit_new(3)));

    /* Evaluate the tree. */
    printf("%d\n", node_eval(tree));
}
