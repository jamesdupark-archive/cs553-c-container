CC = gcc
CFLAGS = -g -Wall -std=gnu99
EXECS = ccontainer
CFILES = container.c

all: $(EXECS)

ccontainer: $(CFILES)
	$(CC) $(CFLAGS) $(CFILES) -o $@

clean:
	rm -rf $(EXECS)