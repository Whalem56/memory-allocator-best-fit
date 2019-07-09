C_FILES := $(wildcard *.c)
TARGETS := ${C_FILES:.c=}

all: ${TARGETS}

%: %.c
	gcc -g -std=gnu99 -m32 -Xlinker -rpath=.. -o $@ $< -L.. -lmem

clean:
	rm -rf ${TARGETS} *.o
