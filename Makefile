card_game: main.o game.o io.o gui.o linked_list.o
	gcc main.o game.o io.o gui.o linked_list.o -o card_game

main.o: main.c game.h
	gcc -c main.c

game.o: game.c game.h io.h gui.h linked_list.h
	gcc -c game.c

io.o: io.c io.h game.h gui.h linked_list.h
	gcc -c io.c

gui.o: gui.c gui.h io.h linked_list.h
	gcc -c gui.c

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c

clean:
	rm main.o game.o io.o gui.o linked_list.o card_game
