#ifndef ITERATOR_H
#define ITERATOR_H

typedef struct CDSIterator CDSIterator;

typedef int (*CDSIteratorHasNextCbk)(void *pData);
typedef int (*CDSIteratorNextCbk)(void *pData, void *dst);
typedef void* (*CDSIteratorNextRefCbk)(void *pData);
typedef void (*CDSIteratorFreeCbk)(void *pData);

CDSIterator* CDSit_alloc(CDSIteratorHasNextCbk hasNextImpl,
                         CDSIteratorNextCbk nextImpl,
                         CDSIteratorNextRefCbk nextRefImpl,
                         CDSIteratorFreeCbk freeImpl,
                         void *pData);

void CDSit_free(CDSIterator *it);

int CDSit_hasNext(CDSIterator *it);

int CDSit_next(CDSIterator *it, void *dst);

void* CDSit_nextRef(CDSIterator *it);
#endif
