#ifndef CDS_ARRAYLIST_H
#define CDS_ARRAYLIST_H

#include "iterator.h"

typedef struct CDSArrayList CDSArrayList;

CDSArrayList* CDSal_alloc(size_t el_size, int n);
void CDSal_free(CDSArrayList *lst);
int CDSal_size(CDSArrayList *lst);
int CDSal_append(CDSArrayList *lst, void *el);
int CDSal_get(CDSArrayList *lst, int idx, void *dst);
int CDSal_remove(CDSArrayList *lst, int idx);
CDSIterator* CDSal_iterator(CDSArrayList *lst);

#endif
