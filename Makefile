CFLAGS += -Wall -std=c99

LD = gcc

CDS_SRCDIR = ./src
TEST_SRCDIR = ./test-src

LIBCDS_OBJS = $(CDS_SRCDIR)/arraylist.o $(CDS_SRCDIR)/iterator.o

libcds.a: $(LIBCDS_OBJS)
	ar rc $@ $^
	ranlib $@

$(TEST_SRCDIR)/%.o: $(TEST_SRCDIR)/%.c
	$(CC) $(CFLAGS) -I $(CDS_SRCDIR) -c -o $@ $<

test: $(TEST_SRCDIR)/test.o libcds.a
	$(LD) $(LDFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(CDS_SRCDIR)/*.o
	rm -f libcds.a
	rm -f $(TEST_SRCDIR)/*.o
	rm -f test