#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "arraylist.h"
#include "list.h"
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

int CDSal_add(CDSArrayList *lst, int idx, void *el) {
  if (idx > lst->arr_next) return 1;
  if (idx < 0) idx %= lst->arr_next;

  if (lst->arr_next >= lst->arr_size) {
    int nsize = lst->arr_size * 2;
    void *narr = realloc(lst->arr, lst->el_size * nsize);
    if (!narr) return 2;
    lst->arr = narr;
    lst->arr_size = nsize;
  }

  uint8_t *arr_idx_ptr;
  if (idx == lst->arr_next) {
    arr_idx_ptr = lst->arr + (lst->el_size * lst->arr_next);
  } else {
    uint8_t *arr_nidx_ptr;
    arr_idx_ptr = lst->arr + (lst->el_size * idx);
    arr_nidx_ptr = lst->arr + (lst->el_size * (idx + 1));
    memmove(arr_nidx_ptr, arr_idx_ptr, lst->arr_next - idx);
  }
  lst->arr_next++;
  return 0;
}

int CDSal_append(CDSArrayList *lst, void *el) {
  return CDSal_add(lst, lst->arr_next, el);
}

int CDSal_get(CDSArrayList *lst, int idx, void *dst) {
  void *src;
  if (idx >= lst->arr_next) return 1;
  if (idx < 0) idx %= lst->arr_next;

  src = (void *)lst->arr + (lst->el_size * idx);
  memcpy(dst, src, lst->el_size);
  return 0;
}

void* CDSal_getRef(CDSArrayList *lst, int idx) {
  if (idx >= lst->arr_next) return NULL;
  if (idx < 0) idx %= lst->arr_next;

  return (void *)lst->arr + (lst->el_size * idx);
}

int CDSal_remove(CDSArrayList *lst, int idx) {
  if (idx >= lst->arr_next) return 1;
  if (idx < 0) idx %= lst->arr_next;
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

CDSArrayList* CDSal_slice(CDSArrayList* lst, int start, int end) {
  int len;
  if (start < 0) start %= lst->arr_next;
  if (end < 0) end %= lst->arr_next;

  if (start >= lst->arr_next || end >= lst->arr_next || start >= end) return NULL;
  len = end - start - 1;
  CDSArrayList *slst =  CDSal_alloc(lst->el_size, len);
  if (!slst) return NULL;

  memcpy(slst->arr, lst->arr + (lst->el_size * start), len * lst->el_size);
  slst->arr_next = len;
  return slst;
}

/*
 * ITERATOR INTERFACE
 */

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

/*
 * LIST INTERFACE
 */

static void* CDSalw_cloneEmpty(void *ilst) {
  CDSArrayList *lst = (CDSArrayList *)ilst;

  return CDSal_alloc(lst->el_size, 1);
}

static void CDSalw_free(void *lst) {
  CDSal_free((CDSArrayList *)lst);
}

static int CDSalw_size(void *lst) {
  return CDSal_size((CDSArrayList *)lst);
}

static int CDSalw_add(void *lst, int idx, void *el) {
  return CDSal_add((CDSArrayList *)lst, idx, el);
}

static int CDSalw_append(void *lst, void *el) {
  return CDSal_append((CDSArrayList *)lst, el);
}

static int CDSalw_get(void *lst, int idx, void *dst) {
  return CDSal_get((CDSArrayList *)lst, idx, dst);
}

static void* CDSalw_getRef(void *lst, int idx) {
  return CDSal_getRef((CDSArrayList *)lst, idx);
}

static int CDSalw_remove(void *lst, int idx) {
  return CDSal_remove((CDSArrayList *)lst, idx);
}

static void* CDSalw_slice(void *lst, int start, int end) {
  return (void*)CDSal_slice((CDSArrayList *)lst, start, end);
}

static CDSIterator* CDSalw_iterator(void *lst) {
  return CDSal_iterator((CDSArrayList *)lst);
}

struct CDSListMethodsDict CDSal_mdict = {
  .cloneEmptyImpl = CDSalw_cloneEmpty,
  .freeImpl = CDSalw_free,
  .sizeImpl = CDSalw_size,
  .addImpl = CDSalw_add,
  .appendImpl = CDSalw_append,
  .getImpl = CDSalw_get,
  .getRefImpl = CDSalw_getRef,
  .removeImpl = CDSalw_remove,
  .sliceImpl = CDSalw_slice,
  .iteratorImpl = CDSalw_iterator
};

CDSList* CDSal_wrapAsList(CDSArrayList *lst) {
  CDSList *lstw = malloc(sizeof(CDSList));
  if (!lstw) return NULL;

  lstw->mdict = &CDSal_mdict;
  lstw->ilst = (void *)lst;
  return lstw;
}
