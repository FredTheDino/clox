CFLAGS=-std=c11 -pedantic

.PHONY: clox default

default: clox

main: *.c

clox: main
	mv main clox

run: clox
	./clox
