quash: main.c
	gcc -Wall -Wextra -Werror -pedantic $^ -o quash

quash_test: quash
	./quash