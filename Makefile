src = $(wildcard src/*.c)
obj = $(src:.c=.o)

CFLAGS = -Ofast -march=native -mtune=native -s -fopenmp -Iinclude
LDFLAGS = -lSDL2 -lGL -ldl

mirv: $(obj)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) mirv
