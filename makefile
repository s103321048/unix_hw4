# makefile
all:main.c
	gcc main.c -o webserver
clean:
	rm -f webserver
