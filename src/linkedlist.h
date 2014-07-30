#ifndef CDS_LINKEDLIST_H
#define CDS_LINKEDLIST_H

#include "iterator.h"
#include "list.h"

typedef struct CDSLinkedList CDSLinkedList;

CDSLinkedList* CDSll_alloc(size_t el_size);
void CDSll_free(CDSLinkedList *lst);
int CDSll_size(CDSLinkedList *lst);
int CDSll_add(CDSLinkedList *lst, int idx, void *el);
int CDSll_append(CDSLinkedList *lst, void *el);
int CDSll_get(CDSLinkedList *lst, int idx, void *dst);
void* CDSll_getRef(CDSLinkedList *lst, int idx);
int CDSll_remove(CDSLinkedList *lst, int idx);
CDSLinkedList* CDSll_slice(CDSLinkedList* lst, int start, int end);
CDSIterator* CDSll_iterator(CDSLinkedList *lst);
CDSList* CDSll_wrapAsList(CDSLinkedList *lst);

#endif
