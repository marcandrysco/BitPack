all: pack-test

pack-test: test.c pack.c pack.h Makefile
	gcc test.c pack.c -o pack-test -O2 -g -Wall -Werror

run: all
	./pack-test

debug: all
	gdb ./pack-test -ex run

clean:
	rm pack-test

dist-clean: clean

.PHONY: all run debug clean dist-clean
