CC = g++
CFLAGS = -g -Wall -Werror

default: SearchNewBooks

Book.o: Book.cc Book.h
	$(CC) $(CFLAGS) -c Book.cc

SearchNewBooks.o: SearchNewBooks.cc
	$(CC) $(CFLAGS) -c SearchNewBooks.cc

SearchNewBooks: Book.o SearchNewBooks.o
	$(CC) $(CFLAGS) -o SearchNewBooks Book.o SearchNewBooks.o

clean:
	$(RM) SearchNewBooks *.o *~