CC = gcc
CFLAGS = -pedantic -Wall
LDFLAGS = -lsx -lm
SRC = main.c vue.c callback.c modele.c
PROG = packman
OBJS = $(SRC:.c=.o)
.SUFFIXES: .c .o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.c modele.h vue.h callback.h
modele.o: modele.c modele.h
vue.o: vue.c vue.h modele.h callback.h
callback.o: callback.c callback.h modele.h vue.h

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o *~ core $(PROG) ./a.out
