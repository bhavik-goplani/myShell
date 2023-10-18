quash: main.c
	gcc -Wall -Wextra -Werror -pedantic main.c execcmd.c -o quash

quash_test: quash
	./quash