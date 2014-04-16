#ifndef ITERATOR_H
#define ITERATOR_H

typedef struct CDSIterator CDSIterator;

typedef int (*CDSIteratorHasNextCbk)(void *);
typedef void *(*CDSIteratorNextCbk)(void *);
typedef void (*CDSIteratorFreeCbk)(void *);

CDSIterator* CDSit_alloc(CDSIteratorHasNextCbk hasNextImpl,
                         CDSIteratorNextCbk nextImpl,
                         CDSIteratorFreeCbk freeImpl,
                         void *pData);

void CDSit_free(CDSIterator *it);

int CDSit_hasNext(CDSIterator *it);

void* CDSit_next(CDSIterator *it);

#endif
