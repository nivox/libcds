#ifndef LIST_H
#define LIST_H

#include "iterator.h"

typedef void* (*CDSListCloneEmptyImpl)(void *ilst);
typedef void (*CDSListFreeImpl)(void *ilst);
typedef int (*CDSListSizeImpl)(void *ilst);
typedef int (*CDSListAddImpl)(void *ilst, int idx, void *el);
typedef int (*CDSListAppendImpl)(void *ilst, void *el);
typedef int (*CDSListGetImpl)(void *ilst, int idx, void *dst);
typedef void* (*CDSListGetRefImpl)(void *ilst, int idx);
typedef int (*CDSListRemoveImpl)(void *ilst, int idx);
typedef void* (*CDSListSliceImpl)(void *ilst, int start, int end);
typedef CDSIterator* (*CDSListIteratorImpl)(void *ilst);

struct CDSListMethodsDict {
  /** Required */
  CDSListCloneEmptyImpl cloneEmptyImpl;
  /** Required */
  CDSListFreeImpl freeImpl;
  /** Required */
  CDSListSizeImpl sizeImpl;
  /** Required */
  CDSListAddImpl addImpl;
  CDSListAppendImpl appendImpl;
  /** Required */
  CDSListGetImpl getImpl;
  /** Required */
  CDSListGetRefImpl getRefImpl;
  /** Required */
  CDSListRemoveImpl removeImpl;
  CDSListSliceImpl sliceImpl;
  /** Required */
  CDSListIteratorImpl iteratorImpl;
};

typedef struct CDSList {
  struct CDSListMethodsDict *mdict;
  void *ilst;
} CDSList;


void CDSl_free(CDSList *lst, int freeInnerLst);
int CDSl_size(CDSList *lst);
int CDSl_add(CDSList *lst, int idx, void *el);
int CDSl_append(CDSList *lst, void *el);
int CDSl_get(CDSList *lst, int idx, void *dst);
void* CDSl_getRef(CDSList *lst, int idx);
int CDSl_remove(CDSList *lst, int idx);
CDSList* CDSl_slice(CDSList* lst, int start, int end);
CDSIterator* CDSl_iterator(CDSList *lst);


#endif
