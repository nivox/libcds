#include <stdio.h>

#include "quickcheck4c.h"

#include "../src/iterator.h"
#include "../src/list.h"
#include "../src/arraylist.h"


typedef CDSList* (*listAlloc)(size_t el_size);
listAlloc currentListAllocator = NULL;

/*************************************************************
 * List generation functions
 *************************************************************/
CDSList* arrayListAlloc(size_t el_size) {
  CDSArrayList *l = CDSal_alloc(el_size, 0);
  return CDSal_wrapAsList(l);
}

CDSList* castToList(void *arr, int n, size_t el_size) {
  CDSList *l = currentListAllocator(el_size);
  int i;
  for (i=0; i<n; i++) {
    CDSl_append(l, arr + (i*el_size));
  }
  return l;
}

/*************************************************************
 * Properties
 *************************************************************/
QCC_TestStatus listAppend(QCC_GenValue **vals, int len, QCC_Stamp **stamps) {
  int *array = QCC_getValue(vals, 0, int*);

  if (vals[0]->n == 0) QCC_label(stamps, "trivial");
  CDSList *l = castToList(array, vals[0]->n, sizeof(int));

  // Testing correct creation
  QCC_TestStatus res = CDSl_size(l) == vals[0]->n;
  int i;
  for (i=0; i<vals[0]->n; i++) {
    res = QCC_and(res, *((int*)CDSl_getRef(l, i)) == array[i]);
  }

  CDSl_free(l, 1);
  return res;
}

QCC_TestStatus listAddHead(QCC_GenValue **vals, int len, QCC_Stamp **stamps) {
  int *array = QCC_getValue(vals, 0, int*);

  if (vals[0]->n == 0) QCC_label(stamps, "trivial");
  CDSList *l = currentListAllocator(sizeof(int));
  int i;
  for (i=0; i<vals[0]->n; i++) {
    CDSl_add(l, 0, &array[i]);
  }

  // Testing correct creation
  QCC_TestStatus res = CDSl_size(l) == vals[0]->n;
  for (i=vals[0]->n-1; i>=0; i--) {
    res = QCC_and(res, *(int*)CDSl_getRef(l, vals[0]->n-1 - i) == array[i]);
  }

  CDSl_free(l, 1);
  return res;
}

QCC_TestStatus listAddScattered(QCC_GenValue **vals, int len, QCC_Stamp **stamps) {
  int *array = QCC_getValue(vals, 0, int*);

  if (vals[0]->n <= 1) QCC_label(stamps, "trivial");

  int allDifferent = 1;
  int i,j;
  for (i=0; i<vals[0]->n; i++) {
    for (j=0; j<vals[0]->n; j++)
      allDifferent = i != j ? array[i] != array[j] : 1;
  }

  if (!allDifferent) return QCC_NOTHING;
  CDSList *l = currentListAllocator(sizeof(int));
  for (i=0; i<vals[0]->n; i++) {
    int idx = 0;
    if (CDSl_size(l) > 0) {
      idx = array[i] % CDSl_size(l);
      idx = idx < 0 ? idx * -1 : idx;
    }
    CDSl_add(l, idx, &array[i]);
  }

  QCC_TestStatus res = CDSl_size(l) == vals[0]->n;
  for (i=0; i<vals[0]->n; i++) {
    int el = *(int*)CDSl_getRef(l, i);
    int found = 0;

    for (j=0; j<vals[0]->n; j++) found = found || el == array[j];
    res = QCC_and(res, found);
  }

  CDSl_free(l, 1);
  return res;
}

QCC_TestStatus listIterationGet(QCC_GenValue **vals, int len, QCC_Stamp **stamps) {
  int *array = QCC_getValue(vals, 0, int*);

  if (vals[0]->n <= 1) QCC_label(stamps, "trivial");
  CDSList *l = castToList(array, vals[0]->n, sizeof(int));

  CDSIterator *it = CDSl_iterator(l);
  int i=0;
  QCC_TestStatus res = QCC_OK;
  while(CDSit_hasNext(it)) {
    int elIt;
    int elGet;
    int *elGetRef;

    CDSit_next(it, &elIt);
    CDSl_get(l, i, &elGet);
    elGetRef = (int*) CDSl_getRef(l, i);

    res = QCC_and(res, elIt == elGet && elGet == *elGetRef);
    i++;
  }
  CDSit_free(it);

  res = QCC_and(res, i == vals[0]->n);
  CDSl_free(l, 1);
  return res;
}

/*************************************************************
 * Main test logic
 *************************************************************/
struct ListImplInfo {
  char *name;
  listAlloc alloc;
};

int main(int argc, char **argv) {
  struct ListImplInfo infoArr[] = {
    (struct ListImplInfo) { .name="ArrayList", .alloc=arrayListAlloc }
  };

  int i;
  QCC_init(0);
  for (i=0; i<sizeof(infoArr)/sizeof(struct ListImplInfo); i++) {
    printf("------------------------------------------------------------\n" \
           "Testing list implementation: %s\n", infoArr[i].name);
    currentListAllocator = infoArr[i].alloc;

    printf("- listAppend:\n");
    QCC_testForAll(100, 1000, listAppend, 1, QCC_genArrayInt);

    printf("- listAddHead:\n");
    QCC_testForAll(100, 1000, listAddHead, 1, QCC_genArrayInt);

    printf("- listAddScattered:\n");
    QCC_testForAll(100, 1000, listAddScattered, 1, QCC_genArrayInt);

    printf("- listIterationGet:\n");
    QCC_testForAll(100, 1000, listIterationGet, 1, QCC_genArrayInt);

  }

  return 0;
}
