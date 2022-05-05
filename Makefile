CFLAGS=-Werror -Wextra
VPATH=src/
SRC=$(wildcard src/*.c)

all: bin/smol-emu

bin/:
	mkdir -p bin/

bin/smol-emu: $(SRC) | bin/
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -r bin/

.PHONY: install
install: bin/smol-emu
	cp bin/smol-emu /usr/local/bin/smol-emu
