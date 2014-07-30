#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"
#include "list.h"
#include "iterator.h"

typedef struct CDSLLEntry {
  void *value;
  struct CDSLLEntry *next;
  struct CDSLLEntry *prev;
} CDSLLEntry;

struct CDSLinkedList {
  size_t el_size;
  int len;
  CDSLLEntry *head;
  CDSLLEntry *tail;
};

struct CDSLinkedListIterator {
  CDSLinkedList *lst;
  CDSLLEntry *e;
};


CDSLinkedList* CDSll_alloc(size_t el_size) {
  CDSLinkedList *lst = malloc(sizeof(CDSLinkedList));
  if (!lst) return NULL;

  lst->el_size = el_size;
  lst->len = 0;
  lst->head = NULL;
  lst->tail = NULL;
  return lst;
}

void CDSll_free(CDSLinkedList *lst) {
  int i;
  CDSLLEntry *e = lst->head;
  CDSLLEntry *n = NULL;
  for (i=0; i<lst->len; i++) {
    n = e->next;
    free(e->value);
    free(e);
    e = n;
  }
  free(lst);
}

int CDSll_size(CDSLinkedList *lst) {
  return lst->len;
}


static CDSLLEntry* CDSll_seekToIdx(CDSLinkedList *lst, int idx, CDSLLEntry **prev, CDSLLEntry **next) {
  CDSLLEntry *e = NULL;
  int i;

  if (idx > lst->len/2) {
    CDSLLEntry *p = lst->tail;
    e = NULL;
    if (next) *next = NULL;
    for (i=lst->len; i>idx; i--) {
      if (next) *next = e;
      e = p;
      p = p->prev;
    }
    if (prev) *prev = p;
  } else {
    e = lst->head;
    if (prev) *prev = NULL;
    for (i=0; i<idx; i++) {
      if (prev) *prev = e;
      e = e->next;
    }
    if (next) *next = e != NULL ? e->next : NULL;
  }

  return e;
}


int CDSll_add(CDSLinkedList *lst, int idx, void *el) {
  if (idx > lst->len) return 1;
  if (idx < 0) {
    idx = lst->len + idx;
    if (idx < 0) return 1;
  }

  CDSLLEntry *prevN = NULL;
  CDSLLEntry *nextN = CDSll_seekToIdx(lst, idx, &prevN, NULL);

  CDSLLEntry *n = malloc(sizeof(CDSLLEntry));
  if (!n) return 1;

  n->value=malloc(lst->el_size);
  if (!n->value) {
    free(n);
    return 1;
  }

  memcpy(n->value, el, lst->el_size);

  if (nextN != NULL) {
    n->next = nextN;
    nextN->prev = n;
  } else {
    lst->tail = n;
    n->next = NULL;
  }

  if (prevN != NULL) {
    prevN->next = n;
    n->prev = prevN;
  } else {
    lst->head = n;
    n->prev = NULL;
  }

  lst->len++;
  return 0;
}

int CDSll_append(CDSLinkedList *lst, void *el) {
  return CDSll_add(lst, lst->len, el);
}

int CDSll_get(CDSLinkedList *lst, int idx, void *dst) {
  void *ptr = CDSll_getRef(lst, idx);
  if (!ptr) return 1;
  memcpy(dst, ptr, lst->el_size);
  return 0;
}

void* CDSll_getRef(CDSLinkedList *lst, int idx) {
  if (idx >= lst->len) return NULL;
  if (idx < 0) {
    idx = lst->len + idx;
    if (idx < 0) return NULL;
  }

  CDSLLEntry *e = CDSll_seekToIdx(lst, idx, NULL, NULL);
  return e->value;
}

int CDSll_remove(CDSLinkedList *lst, int idx) {
  if (idx >= lst->len) return 1;
  if (idx < 0) {
    idx = lst->len + idx;
    if (idx < 0) return 1;
  }

  CDSLLEntry *e = CDSll_seekToIdx(lst, idx, NULL, NULL);

  if (e->prev) e->prev->next = e->next;
  else {
    lst->head = e->next;
    if (e->next) e->next->prev = NULL;
  }

  if (e->next) e->next->prev = e->prev;
  else {
    lst->tail = e->prev;
    if (e->prev) e->prev->next = NULL;
  }

  free(e->value);
  free(e);
  lst->len--;
  return 0;
}

CDSLinkedList* CDSll_slice(CDSLinkedList* lst, int start, int end) {
  if (start >= lst->len) return NULL;
  if (start < 0) {
    start = lst->len + start;
    if (start < 0) return NULL;
  }

  if (end > lst->len)return NULL;
  if (end < 0){
    end = lst->len + end;
    if (end < 0) return NULL;
  }

  if (start >= end) return NULL;
  CDSLinkedList *slst = CDSll_alloc(lst->el_size);

  CDSLLEntry *e = lst->head;
  int i;
  for (i=0; i<end; i++) {
    if (i >= start) CDSll_append(slst, e->value);
    e = e->next;
  }
  return slst;
}

/*
 * ITERATOR INTERFACE
 */

static void CDSll_iteratorFree(void *pData) {
  free(pData);
}

static int CDSll_iteratorHasNext(void *pData) {
  struct CDSLinkedListIterator *d = (struct CDSLinkedListIterator *)pData;

  return d->e != NULL;
}

static int CDSll_iteratorNext(void *pData, void *dst) {
  struct CDSLinkedListIterator *d = (struct CDSLinkedListIterator *)pData;

  if (d->e == NULL) return 1;
  memcpy(dst, d->e->value, d->lst->el_size);
  d->e = d->e->next;
  return 0;
}

static void* CDSll_iteratorNextRef(void *pData) {
  struct CDSLinkedListIterator *d = (struct CDSLinkedListIterator *)pData;

  if (d->e == NULL) return NULL;
  d->e = d->e->next;
  return d->e->value;
}


CDSIterator* CDSll_iterator(CDSLinkedList *lst) {
  struct CDSLinkedListIterator *d = malloc(sizeof(struct CDSLinkedListIterator));
  if (!d) return NULL;

  d->e = lst->head;
  d->lst = lst;

  CDSIterator *i = CDSit_alloc(CDSll_iteratorHasNext,
                               CDSll_iteratorNext,
                               CDSll_iteratorNextRef,
                               CDSll_iteratorFree,
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

static void* CDSllw_cloneEmpty(void *ilst) {
  CDSLinkedList *lst = (CDSLinkedList *)ilst;

  CDSLinkedList *newLst = CDSll_alloc(lst->el_size);
  if (!newLst) return NULL;
  return CDSll_wrapAsList(newLst);
}

static void CDSllw_free(void *lst) {
  CDSll_free((CDSLinkedList *)lst);
}

static int CDSllw_size(void *lst) {
  return CDSll_size((CDSLinkedList *)lst);
}

static int CDSllw_add(void *lst, int idx, void *el) {
  return CDSll_add((CDSLinkedList *)lst, idx, el);
}

static int CDSllw_append(void *lst, void *el) {
  return CDSll_append((CDSLinkedList *)lst, el);
}

static int CDSllw_get(void *lst, int idx, void *dst) {
  return CDSll_get((CDSLinkedList *)lst, idx, dst);
}

static void* CDSllw_getRef(void *lst, int idx) {
  return CDSll_getRef((CDSLinkedList *)lst, idx);
}

static int CDSllw_remove(void *lst, int idx) {
  return CDSll_remove((CDSLinkedList *)lst, idx);
}

static void* CDSllw_slice(void *lst, int start, int end) {
  return (void*)CDSll_slice((CDSLinkedList *)lst, start, end);
}

static CDSIterator* CDSllw_iterator(void *lst) {
  return CDSll_iterator((CDSLinkedList *)lst);
}

static void* CDSllw_wrap(void *lst) {
  return CDSll_wrapAsList((CDSLinkedList *)lst);
}

struct CDSListMethodsDict CDSll_mdict = {
  .cloneEmptyImpl = CDSllw_cloneEmpty,
  .freeImpl = CDSllw_free,
  .sizeImpl = CDSllw_size,
  .addImpl = CDSllw_add,
  .appendImpl = CDSllw_append,
  .getImpl = CDSllw_get,
  .getRefImpl = CDSllw_getRef,
  .removeImpl = CDSllw_remove,
  .sliceImpl = CDSllw_slice,
  .iteratorImpl = CDSllw_iterator,
  .wrapImpl = CDSllw_wrap
};

CDSList* CDSll_wrapAsList(CDSLinkedList *lst) {
  CDSList *lstw = malloc(sizeof(CDSList));
  if (!lstw) return NULL;

  lstw->mdict = &CDSll_mdict;
  lstw->ilst = (void *)lst;
  return lstw;
}
