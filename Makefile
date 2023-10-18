quash: main.c
	gcc -Wall -Wextra -Werror -pedantic *.c -o quash

quash_test: quash
	./quash
	
clean:
	rm -f quash