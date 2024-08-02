#include "tree.h"

Tree *Tree_new_1(Allocator *p_allocator) {
  return Tree_new_3(NULL, NULL, p_allocator);
}

Tree *Tree_new_2(Comparator_Fn compatator, Allocator *p_allocator) {
  return Tree_new_3(NULL, compatator, p_allocator);
}

Tree *Tree_new_3(void (*destructor)(void *), Comparator_Fn comparator,
                 Allocator *p_allocator) {
  bool b_external_allocator = true;
  if (!p_allocator) {
    p_allocator = Allocator_default();
    b_external_allocator = false;
  }
  Tree *p_tree = Allocator_alloc(p_allocator, sizeof(Tree));
  if (!p_tree) {
    Allocator_free_everything(p_allocator);
    return NULL;
  }

  p_tree->p_root = NULL;
  p_tree->destructor = destructor;
  p_tree->comparator = comparator;
  p_tree->p_allocator = p_allocator;
  p_tree->b_external_allocator = b_external_allocator;

  return p_tree;
}

Tree *Tree_new_5(void *p_arr_data, size_t u_arr_size,
                 void (*destructor)(void *), Comparator_Fn comparator,
                 Allocator *p_allocator) {
  Tree *p_tree = Tree_new_3(destructor, comparator, p_allocator);
  if (!p_tree) {
    return NULL;
  }

  for (size_t i = 0; i < u_arr_size; i++) {
    Tree_insert(p_tree, p_arr_data + i);
  }

  return p_tree;
}

void Tree_free_node(Tree_Node *p_node, void (*destructor)(void *),
                    Allocator *p_allocator) {
  if (!p_node) {
    return;
  }

  Tree_free_node(p_node->p_left, destructor, p_allocator);
  Tree_free_node(p_node->p_right, destructor, p_allocator);

  if (destructor) {
    destructor(p_node->p_data);
  }

  Allocator_free(p_allocator, p_node);
}

#if CUTIL_AUTO_CLEANUP_TYPES
void Tree_free_(Tree **p_tree) {
  if (!p_tree) {
    return;
  }
  Tree_free(*p_tree);
  *p_tree = NULL;
}
#endif

void Tree_free(Tree *p_tree) {
  if (!p_tree) {
    return;
  }

  Tree_free_node(p_tree->p_root, p_tree->destructor, p_tree->p_allocator);

  if (!p_tree->b_external_allocator) {
    Allocator_free_everything(p_tree->p_allocator);
  }
}

void Tree_insert(Tree *p_tree, void *p_data) {
  if (!p_tree) {
    return;
  }

  Tree_Node *p_node = Allocator_alloc(p_tree->p_allocator, sizeof(Tree_Node));
  if (!p_node) {
    return;
  }

  p_node->p_data = p_data;
  p_node->p_left = NULL;
  p_node->p_right = NULL;

  if (!p_tree->p_root) {
    p_tree->p_root = p_node;
    return;
  }

  Tree_Node *p_current = p_tree->p_root;
  while (true) {
    size_t comparison;

    if (p_tree->comparator) {
      comparison = p_tree->comparator(p_data, p_current->p_data);
    } else {
      comparison = ((size_t)p_data < (size_t)p_current->p_data)   ? -1
                   : ((size_t)p_data > (size_t)p_current->p_data) ? 1
                                                                  : 0;
    }

    if (comparison < 0) {
      if (p_current->p_left) {
        p_current = p_current->p_left;
      } else {
        p_current->p_left = p_node;
        return;
      }
    } else if (comparison > 0) {
      if (p_current->p_right) {
        p_current = p_current->p_right;
      } else {
        p_current->p_right = p_node;
        return;
      }
    } else {
      // Handle duplicate key
      // Option 1: Overwrite existing data
      // p_current->p_data = p_data;
      // Allocator_free(p_tree->p_allocator, p_node);

      // Option 2: Insert as left child (chosen here)
      p_node->p_left = p_current->p_left;
      p_current->p_left = p_node;

      return;
    }
  }
}
void *Tree_find(Tree *p_tree, void *p_data) {
  if (!p_tree || !p_tree->p_root) {
    return NULL;
  }

  Tree_Node *p_current = p_tree->p_root;
  while (p_current) {
    if (p_tree->comparator(p_data, p_current->p_data)) {
      p_current = p_current->p_left;
    } else if (p_tree->comparator(p_current->p_data, p_data)) {
      p_current = p_current->p_right;
    } else {
      return p_current->p_data;
    }
  }

  return NULL;
}

void Tree_clear(Tree *p_tree) {
  if (!p_tree) {
    return;
  }

  Tree_free_node(p_tree->p_root, p_tree->destructor, p_tree->p_allocator);
  p_tree->p_root = NULL;
}

bool Tree_empty(Tree *p_tree) { return !p_tree || !p_tree->p_root; }
