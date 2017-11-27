CC = clang
CFLAGS = -Wall -pedantic -std=c99

OBJECTS = $(filter-out main.o,$(patsubst %.c,%.o,$(wildcard *.c)))
BIN = fdstat
PREFIX = /usr/bin

ifdef DEBUG
CFLAGS += -DDEBUG -g
else
CFLAGS += -O3
endif


$(BIN): main.c $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(BIN)

%.o: %.c %.h $(OTHER)
	$(CC) -c $(CFLAGS) $< -o $@

install: $(BIN)
	install -D -m 0755 $(BIN) $(DESTDIR)$(PREFIX)/$(BIN)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/$(BIN)

clean:
	rm -fr $(BIN) *.o

.PHONY: debian
debian:
	dpkg-buildpackage -b -uc -us
