#ifndef CDS_ARRAYLIST_H
#define CDS_ARRAYLIST_H

#include "iterator.h"

typedef struct CDSArrayList CDSArrayList;

CDSArrayList* CDSal_alloc(size_t el_size, int n);
void CDSal_free(CDSArrayList *lst);
int CDSal_size(CDSArrayList *lst);
int CDSal_add(CDSArrayList *lst, int idx, void *el);
int CDSal_append(CDSArrayList *lst, void *el);
void* CDSal_getRef(CDSArrayList *lst, int idx);
int CDSal_remove(CDSArrayList *lst, int idx);
CDSArrayList* CDSal_slice(CDSArrayList* lst, int start, int end);
CDSIterator* CDSal_iterator(CDSArrayList *lst);
CDSList* CDSal_wrapAsList(CDSArrayList *lst);

#endif
