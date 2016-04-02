.PHONY: test speck_libs valgrind clean

BIN ?= punch
CC_FLAGS ?= -Werror -Weverything -std=c1x -fPIC
BIN_CC_FLAGS ?= -O3
SPECK_LIBS=$(patsubst %.h, %.o, $(wildcard *.h))
SPECK_PATH = vendor/speck

$(BIN): *.c *.h
	$(CC) $(CC_FLAGS) $(BIN_CC_FLAGS) -o $(BIN) *.c

%.o: %.c %.h
	$(CC) $(CC_FLAGS) -c -g -o $@ $<

-include vendor/speck/speck.mk
test: $(SPECK) $(SPECK_LIBS) $(SUITES)
	@$(SPECK) -f

spec/%.so: spec/spec_helper.h

valgrind: $(BIN)
	@valgrind --leak-check=full --error-exitcode=1 ./$(BIN)
	@valgrind --leak-check=full --error-exitcode=1 ./$(SPECK)

clean:
	@rm -r $(BIN) $(SPECK) $(SPECK).dSYM *.o **/*.so *.dSYM **/*.dSYM || true
