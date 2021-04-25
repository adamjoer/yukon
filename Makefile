card_game:
	gcc-10 game.c -c -o cmake-build-debug/game.o
	gcc-10 linked_list.c -c -o cmake-build-debug/linked_list.o
	gcc-10 gui.c -c -o cmake-build-debug/gui.o
	gcc-10 io.c -c -o cmake-build-debug/io.o
	gcc-10 main.c -c -o cmake-build-debug/main.o
	gcc-10 cmake-build-debug/main.o cmake-build-debug/game.o cmake-build-debug/linked_list.o cmake-build-debug/gui.o cmake-build-debug/io.o -o card_game
