CC=gcc
OPTS=-g -std=c99 -Werror

# NOTE: I had to move the -lm flag to the end to get my code to compile with
# the math function pow for some reason.
all: main.o predictor.o
	$(CC) $(OPTS) -o predictor main.o predictor.o -lm

main.o: main.c predictor.h
	$(CC) $(OPTS) -c main.c

predictor.o: predictor.h predictor.c
	$(CC) $(OPTS) -c predictor.c

clean:
	rm -f *.o predictor;

.PHONY: test-static
test-static: all
	bunzip2 -kc ../traces/int_1.bz2 | ./predictor --static

.PHONY: test-gshare
test-gshare: all
	bunzip2 -kc ../traces/int_1.bz2 | ./predictor --gshare:13

.PHONY: test-tournament
test-tournament: all
	bunzip2 -kc ../traces/int_1.bz2 | ./predictor --tournament:9:10:10

test-custom: all
	bunzip2 -kc ../traces/int_1.bz2 | ./predictor --custom

.PHONY: test-all
test-all: all
	./../tests/static_testing_script.sh
	./../tests/gshare_testing_script.sh
	./../tests/tournament_testing_script.sh
	./../tests/custom_testing_script.sh
