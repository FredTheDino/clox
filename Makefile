CFLAGS=-std=c11 -Wpedantic -g

.PHONY: clox default

default: clox

main: *.c

clox: main
	mv main clox

run: clox
	./clox
