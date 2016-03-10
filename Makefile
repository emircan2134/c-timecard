.PHONY: test speck_libs valgrind clean

BIN ?= punch
CC_FLAGS ?= -Werror -Wall -Wextra -std=c1x -fPIC -D_GNU_SOURCE
BIN_CC_FLAGS ?= -O3
SPECK_LIBS = $(patsubst src/%.h, src/%.o, $(wildcard src/*.h))
SPECK_CFLAGS ?= -Isrc -D_GNU_SOURCE
SPECK_PATH = vendor/speck

$(BIN): src/*.c src/*.h
	$(CC) $(CC_FLAGS) $(BIN_CC_FLAGS) -o $(BIN) src/*.c

src/%.o: src/%.c src/%.h
	$(CC) $(CC_FLAGS) -c -g -o $@ $<

-include vendor/speck/speck.mk
test: $(SPECK) $(SPECK_LIBS) $(SUITES)
	@$(SPECK) -f

spec/%.so: spec/spec_helper.h

valgrind: $(BIN)
	@valgrind --leak-check=full --error-exitcode=1 ./$(BIN)
	@valgrind --leak-check=full --error-exitcode=1 ./$(SPECK)

clean:
	@rm -rf $(BIN) $(SPECK) **/*.o **/*.so **/*.dSYM || true

bootstrap:
	@git submodule update --init --recursive
