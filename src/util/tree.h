#ifndef CUITLL_TREE_H
#define CUITLL_TREE_H

#include "alloc.h"

typedef struct Tree_Node {
  void *p_data;
  struct Tree_Node *p_left;
  struct Tree_Node *p_right;
} Tree_Node;

typedef size_t (*Comparator_Fn)(void *, void *);

typedef struct {
  Tree_Node *p_root;
  Comparator_Fn comparator;
  void (*destructor)(void *);
  bool b_external_allocator;
  Allocator *p_allocator;
} Tree;

#define Tree(type) defer(Tree_free_) Tree *
#define Tree_new(...) CONCAT(Tree_new_, NARGS(__VA_ARGS__))(__VA_ARGS__)

void Tree_free_node(Tree_Node *p_node, Dealloc_Fn destructor,
                    Allocator *p_allocator);

Tree *Tree_new_1(Allocator *p_allocator);
Tree *Tree_new_2(Comparator_Fn comparator, Allocator *p_allocator);
Tree *Tree_new_3(Dealloc_Fn destructor, Comparator_Fn comparator,
                 Allocator *p_allocator);
Tree *Tree_new_5(void *p_arr_data, size_t u_arr_size,
                 Dealloc_Fn destructor, Comparator_Fn comparator,
                 Allocator *p_allocator);
void Tree_free(Tree *p_tree);
void Tree_free_(Tree **pp_tree);
void Tree_insert(Tree *p_tree, void *p_data);
void Tree_remove(Tree *p_tree, void *p_data);
void *Tree_find(Tree *p_tree, void *p_data);
void Tree_clear(Tree *p_tree);
bool Tree_empty(Tree *p_tree);

#endif
