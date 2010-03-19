ALGOS = algorithms/custom.c algorithms/web.c algorithms/lib.c
SRCS = utils.c main.c algorithms.c $(ALGOS)
OBJS = $(SRCS:.c=.o)
TARGET = sort-bench
CC = gcc
LDFLAGS = $(CFLAGS) -L.
COUTFLAGS = -o
CFLAGS = $(warnflags) $(optflags)
warnflags = -Wall
optflags = -O2
debugflags = -g
STEPS = *.o *.so $(TARGET)
TEMPS = .*.swp

all: $(TARGET) Makefile

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) $(COUTFLAGS) $@ -c $<

clean:
	rm -rf $(STEPS)

dist-clean:
	rm -rf $(STEPS) $(TEMPS)

test: $(TARGET)
	@./$(TARGET) -q -t 10000 cases/random-4000 && echo "pass" || echo "fail"
