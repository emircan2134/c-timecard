.PHONY: test speck_libs valgrind clean

BIN ?= punch
CFLAGS ?= -Werror -Wall -Wextra -std=c1x -fPIC -D_GNU_SOURCE
BIN_CFLAGS ?= -O3
SPECK_LIBS = $(patsubst src/%.h, src/%.o, $(wildcard src/*.h))
SPECK_CFLAGS ?= -Isrc -D_GNU_SOURCE
SPECK_PATH = vendor/speck

$(BIN): src/*.c src/*.h
	$(CC) $(CFLAGS) $(BIN_CFLAGS) -o $(BIN) src/*.c

$(BIN).debug: src/*.c src/*.h
	$(CC) $(CFLAGS) -g -o $(BIN).debug src/*.c

src/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c -g -o $@ $<

-include vendor/speck/speck.mk
test: $(SPECK) $(SPECK_LIBS) $(SUITES)
	@$(SPECK) -f

spec/%.so: spec/spec_helper.h

valgrind: TEST_HOME := $(shell mktemp -d -t punch-valgrind)
valgrind: $(BIN).debug
	export HOME=$(TEST_HOME) && valgrind ./$(BIN).debug --help
	export HOME=$(TEST_HOME) && valgrind ./$(BIN).debug in foo
	export HOME=$(TEST_HOME) && valgrind ./$(BIN).debug out
	export HOME=$(TEST_HOME) && valgrind ./$(BIN).debug summary

clean:
	@rm -rf $(BIN) $(BIN).debug $(SPECK) **/*.o **/*.so **/*.dSYM || true

bootstrap:
	@git submodule update --init --recursive
