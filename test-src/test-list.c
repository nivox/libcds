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
    int el = *(int*)CDSl_getRef(l, vals[0]->n-1 - i);
    res = QCC_and(res, el == array[i]);
  }

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
  }
}
