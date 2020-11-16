all: build

build: editor.o
	gcc -o editor editor.o

editor.o: editor.c
	gcc -c editor.c

clean:
	rm -rf *.o
