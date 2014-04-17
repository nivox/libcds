#include <stdio.h>
#include "arraylist.h"

int main(int argc, char **argv) {
  CDSIterator *it;
  CDSArrayList *lst = CDSal_alloc(sizeof(int), 3);

  int val = 3;
  CDSal_append(lst, &val);
  printf("Size: %d\n", CDSal_size(lst));

  val = 7;
  CDSal_append(lst, &val);
  printf("Size: %d\n", CDSal_size(lst));

  val = 9;
  CDSal_append(lst, &val);
  printf("Size: %d\n", CDSal_size(lst));

  val = 17;
  CDSal_append(lst, &val);
  printf("Size: %d\n", CDSal_size(lst));

  printf("After add\n");
  it = CDSal_iterator(lst);
  while(CDSit_hasNext(it)) {
    int v;
    CDSit_next(it, &v);
    printf("Idx %d\n", v);
  }
  CDSit_free(it);

  printf("\nRemove 2\n");
  CDSal_remove(lst, 2);
  it = CDSal_iterator(lst);
  while(CDSit_hasNext(it)) {
    int v;
    CDSit_next(it, &v);
    printf("Idx %d\n", v);
  }
  CDSit_free(it);

  printf("\nRemove end\n");
  CDSal_remove(lst, CDSal_size(lst)-1);
  it = CDSal_iterator(lst);
  while(CDSit_hasNext(it)) {
    int v;
    CDSit_next(it, &v);
    printf("Idx %d\n", v);
  }
  CDSit_free(it);

  printf("\nRemove 0\n");
  CDSal_remove(lst, 0);
  it = CDSal_iterator(lst);
  while(CDSit_hasNext(it)) {
    int v;
    CDSit_next(it, &v);
    printf("Idx %d\n", v);
  }
  CDSit_free(it);

  CDSal_free(lst);
}
