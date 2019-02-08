make:
	gcc -c str-main.c -o str-main.o
	gcc -c strlib.c -o strlib.o
	gcc str-main.o strlib.o -o str-final.o
	rm str-main.o
	rm strlib.o
clean:
	rm str-final.o
