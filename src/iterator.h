#ifndef ITERATOR_H
#define ITERATOR_H

typedef struct CDSIterator CDSIterator;

typedef int (*CDSIteratorHasNextCbk)(void *pData);
typedef int (*CDSIteratorNextCbk)(void *pData, void *dst);
typedef void (*CDSIteratorFreeCbk)(void *pData);

CDSIterator* CDSit_alloc(CDSIteratorHasNextCbk hasNextImpl,
                         CDSIteratorNextCbk nextImpl,
                         CDSIteratorFreeCbk freeImpl,
                         void *pData);

void CDSit_free(CDSIterator *it);

int CDSit_hasNext(CDSIterator *it);

int CDSit_next(CDSIterator *it, void *dst);

#endif
