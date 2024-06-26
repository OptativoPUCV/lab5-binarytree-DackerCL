#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->lower_than = lower_than;
    new->root = NULL;
    new->current = NULL;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree->root == NULL) {
        tree->root = createTreeNode(key, value);
        return;
    }
    TreeNode * aux = tree->root;
    TreeNode * parent = NULL;
    while (aux !=NULL){
        parent = aux;
        if(tree->lower_than(key,aux->pair->key)){
            aux = aux->left;
        }
        else if(tree->lower_than(aux->pair->key, key)){
            aux = aux->right;
        }
        else return;
    }
    TreeNode *nuevo = createTreeNode(key, value);
    nuevo->parent = parent;
    if(parent == NULL){
        tree->root = nuevo;
    }
    else if(tree->lower_than(key,parent->pair->key)){
        parent->left = nuevo;
    }
    else{
        parent->right = nuevo;
    }
    tree->current = nuevo;
}


TreeNode * minimum(TreeNode * x){
    while (x->left != NULL){
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL){
        return;
    }
    if (node->left == NULL && node->right == NULL){
        if (node->parent == NULL){
            tree->root = NULL;
        }else if (node->parent->left == node){
            node->parent->left = NULL;
        }else{
            node->parent->right = NULL;
        }
        tree->current = node->parent;
        free(node);
        return;
    }

    if (node->left == NULL || node->right == NULL){
        TreeNode *child = (node->left != NULL) ? node->left : node->right;
        if (node->parent == NULL){
            tree->root = child;
        }else if (node->parent->left == node){
            node->parent->left = child;
        }else{
            node->parent->right = child;
        }
        child->parent = node->parent;
        tree->current = child;
        free(node);
        return;
    }

    TreeNode *aux = minimum(node->right);
    node->pair->key = aux->pair->key;
    node->pair->value = aux->pair->value;
    removeNode(tree, aux);
    
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;
    if (searchTreeMap(tree,key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}


Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * aux = tree->root;
    while(aux !=NULL){
        if(is_equal(tree,key,aux->pair->key)){
            tree->current = aux;
            return aux->pair;
        }if(tree->lower_than(key,aux->pair->key)){
            aux = aux->left;
        }else{
            aux = aux->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode * aux = tree->root;
    TreeNode * ub = NULL;
    while(aux !=NULL){
        if(is_equal(tree,key,aux->pair->key)){
            tree->current = aux;
            return aux->pair;
        }
        if(tree->lower_than(key, aux->pair->key)){
            ub = aux;
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }
    if(ub != NULL){
        return ub->pair;
    }
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * node = tree->root;
    while(node->left != NULL){
        node = node->left;
    }
    tree->current = node;
    return node->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if(tree->current == NULL) return NULL;
    TreeNode * aux = tree->current;
    
    if(aux->right != NULL){
        tree->current = minimum(aux->right);
        return tree->current->pair;
    }
    
    TreeNode * parent = aux->parent;
    while(parent != NULL && aux == parent->right){
        aux = parent;
        parent = parent->parent;
    }
    tree->current = parent;

    if(parent == NULL){
        return NULL;
    }else{
        return parent->pair;
    }
}
