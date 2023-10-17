quash: main.c
	gcc -Wall -Wextra -Werror -pedantic main.c -o quash

quash_test: quash
	./quash