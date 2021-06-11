all:
	gcc -g -o main src/main.c src/avl.c src/site.c src/googlebot.c src/palavra_chave.c -I./includes
run:
	./main