CC = gcc
CFLAGS = -g -Wall -std=gnu99
BENCHFLAGS = -D BENCHMARK
BENCHFLAGS += -Wl,--no-as-needed,-lprofiler,--as-needed
EXECS = ccontainer bench
CFILES = container.c

all: $(EXECS)

bench: $(CFILES)
	$(CC) $(CFLAGS) $(BENCHFLAGS) $(CFILES) -o $@

ccontainer: $(CFILES)
	$(CC) $(CFLAGS) $(CFILES) -o $@

clean:
	rm -rf $(EXECS)