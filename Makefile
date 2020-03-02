CC = gcc
INCLUDE := -I./
CFLAGS := -g -Wall $(INCLUDE)

PROGNAME = test

all: $(PROGNAME)

.c.o:
	$(CC) $< -o $@

$(PROGNAME): libpciecfg.h

clean:
	rm -rf *.o
	rm -rf $(PROGNAME)
