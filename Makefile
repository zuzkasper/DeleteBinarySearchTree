all : delete.c
	gcc -Wall -Werror -fsanitize=address -o delete delete.c
clean :
	rm -rf delete
