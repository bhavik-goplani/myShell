quash: main.c
	gcc -Wall -Wextra -Werror -pedantic *.c -o quash

quash_test: quash
	./quash
	
clean:
	rm -f quash

quash_valgrind: quash
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s ./quash