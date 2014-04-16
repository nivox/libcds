#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "arraylist.h"

struct CDSArrayList {
  size_t el_size;
  int arr_size;
  int arr_next;
  uint8_t *arr;
};

CDSArrayList* CDSal_alloc(size_t el_size, int n) {
  CDSArrayList *lst = malloc(sizeof(CDSArrayList));
  if (!lst) return NULL;

  lst->arr = (uint8_t *) malloc(el_size * n);
  if (!lst->arr) {
    free(lst);
    return NULL;
  }

  lst->el_size = el_size;
  lst->arr_size = n;
  lst->arr_next = 0;

  return lst;
}

void CDSal_free(CDSArrayList *lst) {
  free(lst->arr);
  free(lst);
}

int CDSal_size(CDSArrayList *lst) {
  return lst->arr_next;
}

int CDSal_append(CDSArrayList *lst, void *el) {
  if (lst->arr_next >= lst->arr_size) {
    int nsize = lst->arr_size * 2;
    void *narr = realloc(lst->arr, lst->el_size * nsize);
    if (!narr) return 1;
    lst->arr = narr;
    lst->arr_size = nsize;
  }

  uint8_t *arr_idx_ptr = lst->arr + (lst->el_size * lst->arr_next);
  memcpy(arr_idx_ptr, el, lst->el_size);
  lst->arr_next++;
  return 0;
}

void* CDSal_get(CDSArrayList *lst, int idx) {
  if (idx < 0 || idx >= lst->arr_next) return NULL;
  return (void *)lst->arr + (lst->el_size * idx);
}
