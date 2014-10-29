main: main.c
			gcc game.c file_operations.c movement.c mr_do_menus.c sprites.c main.c lists.c -o main -lmenu -lncurses -Wall -Wredundant-decls -Wnested-externs -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual -Wundef -Wno-pointer-sign
tests: tests.c
			gcc game.c file_operations.c movement.c mr_do_menus.c sprites.c tests.c lists.c -o tests -lcheck -lmenu -lncurses -Wall -Wredundant-decls -Wnested-externs -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual -Wundef -Wno-pointer-sign
clean:
	    rm main

.PHONY: main tests
