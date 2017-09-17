CC = gcc
CFLAGS = -Wall -pedantic -std=c99

OBJECTS = $(filter-out main.o,$(patsubst %.c,%.o,$(wildcard *.c)))
# OTHER = strconst.h
BIN = fdstat

ifdef DEBUG
CFLAGS += -DDEBUG -g
else
CFLAGS += -O3
endif


$(BIN): main.c $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(BIN)

%.o: %.c %.h $(OTHER)
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm -fr $(BIN) *.o
