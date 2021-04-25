card_game:
	gcc-10 game.c -c -o game.o
	gcc-10 linked_list.c -c -o linked_list.o
	gcc-10 gui.c -c -o gui.o
	gcc-10 io.c -c -o io.o
	gcc-10 main.c -c -o main.o
	gcc-10 main.o game.o linked_list.o gui.o io.o -o card_game
	rm game.o linked_list.o gui.o io.o main.o