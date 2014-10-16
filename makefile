main: main.c
			gcc *.c -o main -lmenu -lncurses -Wall -Wredundant-decls -Wnested-externs -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual -Wundef -Wno-pointer-sign
clean:
	    rm main
