CC     = gcc
CFLAGS = -std=c99

n1: gdb_handle.c
	$(CC) $(CFLAGS) gdb_handle.c -o handle

g1: gdb_handle.c
	$(CC) $(CFLAGS) -g gdb_handle.c -o gdb_handle

clean:
	rm -f a.out handle gdb_handle
