CFLAGS += -Wall -std=c99 -g

LD = gcc

CDS_SRCDIR = ./src
TEST_SRCDIR = ./test-src

TESTS = test-list

LIBCDS_OBJS = $(CDS_SRCDIR)/list.o $(CDS_SRCDIR)/arraylist.o $(CDS_SRCDIR)/iterator.o

libcds.a: $(LIBCDS_OBJS)
	ar rc $@ $^
	ranlib $@

$(TEST_SRCDIR)/%.o: $(TEST_SRCDIR)/%.c
	$(CC) $(CFLAGS) -I $(CDS_SRCDIR) -c -o $@ $<

test: $(TEST_SRCDIR)/test.o libcds.a
	$(LD) $(LDFLAGS) -o $@ $^

.PHONY: tests
tests: $(TESTS)

test-list: $(TEST_SRCDIR)/test-list.o $(TEST_SRCDIR)/quickcheck4c.o libcds.a
	$(LD) $(LDFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(CDS_SRCDIR)/*.o
	rm -f libcds.a
	rm -f $(TEST_SRCDIR)/*.o
	rm -f test $(TESTS)