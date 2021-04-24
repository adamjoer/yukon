card_game:
	gcc-10 linked_list.c -c -o linked_list.o
	gcc-10 main.c -c -o main.o
	gcc-10 main.o linked_list.o -o card_game
	rm linked_list.o main.o