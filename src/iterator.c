#include <stdlib.h>
#include "iterator.h"

struct CDSIterator {
  CDSIteratorHasNextCbk hasNextImpl;
  CDSIteratorNextCbk nextImpl;
  CDSIteratorFreeCbk freeImpl;
  void *pData;
};

CDSIterator* CDSit_alloc(CDSIteratorHasNextCbk hasNextImpl,
                         CDSIteratorNextCbk nextImpl,
                         CDSIteratorFreeCbk freeImpl,
                         void *pData)
{
  CDSIterator *it = malloc(sizeof(CDSIterator));
  if (!it) return NULL;

  it->hasNextImpl = hasNextImpl;
  it->nextImpl = nextImpl;
  it->freeImpl = freeImpl;
  it->pData = pData;

  return it;
}

void CDSit_free(CDSIterator *it) {
  it->freeImpl(it->pData);
  free(it);
}

int CDSit_hasNext(CDSIterator *it) {
  return it->hasNextImpl(it->pData);
}

int CDSit_next(CDSIterator *it, void *dst) {
  return it->nextImpl(it->pData, dst);
}
