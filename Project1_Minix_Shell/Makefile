CC=clang
CFLAGS=-I.
DEPS = 

shell.o: shell.c
	cc -o shell.o -c shell.c

queue.o: queue.c
	cc -o queue.o -c queue.c

parser.o: parser.c
	cc -o parser.o -c parser.c

l_list.o: l_list.c
	cc -o l_list.o -c l_list.c

file_handle.o: file_handle.c
	cc -o file_handle.o -c file_handle.c

calc.o: calc.c
	cc -o calc.o -c calc.c

read_profile.o: read_profile.c
	cc -o read_profile.o -c read_profile.c

myshell: shell.o read_profile.o l_list.o file_handle.o calc.o queue.o parser.o
		cc -o myshell shell.o l_list.o file_handle.o calc.o read_profile.o queue.o parser.o -I.

clean:
	rm myshell shell.o read_profile.o l_list.o file_handle.o calc.o queue.o parser.o
