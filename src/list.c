#include "list.h"

#include <stdlib.h>

void CDSl_free(CDSList *lst, int freeInnerLst) {
  if (freeInnerLst) lst->mdict->freeImpl(lst->ilst);
  free(lst);
}

int CDSl_size(CDSList *lst) {
  return lst->mdict->sizeImpl(lst->ilst);
}

int CDSl_add(CDSList *lst, int idx, void *el) {
  return lst->mdict->addImpl(lst->ilst, idx, el);
}

int CDSl_append(CDSList *lst, void *el) {
  if (lst->mdict->appendImpl) {
    return lst->mdict->appendImpl(lst->ilst, el);
  } else {
    return lst->mdict->addImpl(lst->ilst,
                               lst->mdict->sizeImpl(lst->ilst),
                               el);
  }
}

int CDSl_get(CDSList *lst, int idx, void *dst) {
  return lst->mdict->getImpl(lst->ilst, idx, dst);
}

void* CDSl_getRef(CDSList *lst, int idx) {
  return lst->mdict->getRefImpl(lst->ilst, idx);
}

int CDSl_remove(CDSList *lst, int idx) {
  return lst->mdict->removeImpl(lst->ilst, idx);
}

CDSList* CDSl_slice(CDSList* lst, int start, int end) {
  if (lst->mdict->sliceImpl) {
    void *ilst = lst->mdict->sliceImpl(lst->ilst, start, end);
    return lst->mdict->wrapImpl(ilst);
  } else {
    if (start < 0 || end < 0) {
      int lst_size = CDSl_size(lst);

      if (start < 0) start %= lst_size;
      if (end < 0) end %= lst_size;
    }

    CDSList *slst = lst->mdict->cloneEmptyImpl(lst->ilst);
    if (!slst) return NULL;

    CDSIterator *it = CDSl_iterator(lst);
    int i=0;
    void *e;
    while(CDSit_hasNext(it) && i<end) {
      e = CDSit_nextRef(it);
      if (i > start) CDSl_append(slst, e);
      i++;
    }
    return slst;
  }
}

CDSIterator* CDSl_iterator(CDSList *lst) {
  return lst->mdict->iteratorImpl(lst->ilst);
}
