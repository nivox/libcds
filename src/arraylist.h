#ifndef CDS_ARRAYLIST_H
#define CDS_ARRAYLIST_H

typedef struct CDSArrayList CDSArrayList;

CDSArrayList* CDSal_alloc(size_t el_size, int n);
void CDSal_free(CDSArrayList *lst);
int CDSal_size(CDSArrayList *lst);
int CDSal_append(CDSArrayList *lst, void *el);
void* CDSal_get(CDSArrayList *lst, int idx);

#endif
