#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "arraylist.h"
#include "iterator.h"

struct CDSArrayList {
  size_t el_size;
  int arr_size;
  int arr_next;
  uint8_t *arr;
};

struct CDSArrayListIterator {
  CDSArrayList *lst;
  int next_idx;
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

int CDSal_get(CDSArrayList *lst, int idx, void *dst) {
  void *src;
  if (idx < 0 || idx >= lst->arr_next) return 1;

  src = (void *)lst->arr + (lst->el_size * idx);
  memcpy(dst, src, lst->el_size);
  return 0;
}

int CDSal_remove(CDSArrayList *lst, int idx) {
  if (idx < 0 || idx >= lst->arr_next) return 1;
  if (idx != lst->arr_next - 1) {
    void *rem_el;
    void *next_el;

    rem_el = (void *)lst->arr + (lst->el_size * idx);
    next_el = (void *)lst->arr + (lst->el_size * (idx + 1));

    memmove(rem_el, next_el, (lst->arr_next - idx - 1) * lst->el_size);
  }
  lst->arr_next--;
  return 0;
}

static void CDSal_iteratorFree(void *pData) {
  free(pData);
}

static int CDSal_iteratorHasNext(void *pData) {
  struct CDSArrayListIterator *d = (struct CDSArrayListIterator *)pData;

  return d->next_idx < CDSal_size(d->lst);
}

static int CDSal_iteratorNext(void *pData, void *dst) {
  struct CDSArrayListIterator *d = (struct CDSArrayListIterator *)pData;

  return CDSal_get(d->lst, d->next_idx++, dst);
}

CDSIterator* CDSal_iterator(CDSArrayList *lst) {
  struct CDSArrayListIterator *d = malloc(sizeof(struct CDSArrayListIterator));
  if (!d) return NULL;

  d->lst = lst;
  d->next_idx = 0;

  CDSIterator *i = CDSit_alloc(CDSal_iteratorHasNext,
                               CDSal_iteratorNext,
                               CDSal_iteratorFree,
                               d);
  if (!i) {
    free(d);
    return NULL;
  } else {
    return i;
  }
}
