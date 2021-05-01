card_game: main.o linked_list.o gui.o io.o game.o
	gcc main.o linked_list.o gui.o io.o game.o -o foo

main.o: main.c linked_list.h gui.h io.h game.h
	gcc -c main.c

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c

gui.o: gui.c gui.h game.h
	gcc -c gui.c

io.o: io.c io.h game.h
	gcc -c io.c

game.o: game.c game.h
	gcc -c game.c
	
clean:
	rm foo main.o linked_list.o gui.o io.o game.o
