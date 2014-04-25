#include "list.h"

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
    return lst->mdict->sliceImpl(lst->ilst, start, end);
  } else {
    CDSList *slst = malloc(sizeof(CDSList));
    slst->ilst->cloneEmptyImpl(lst->ilst);
    if (!slst->mdict) {
      free(slst);
      return NULL;
    }
    slst->mdict = lst->mdict;
    return slst;
  }
}

CDSIterator* CDSl_iterator(CDSList *lst) {
  return lst->mdict->iteratorImpl(lst->ilst);
}
