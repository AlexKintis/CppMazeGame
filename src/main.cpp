/*
 *    cs161188
 *    Kintis Alexandros
 *
 *    Final Project
 *
 *    5.9.16-1-MANJARO
 *    gcc version 10.2.0 (GCC)
 *    [doom emacs with lsp-mode(Irony-Mode)]
 *    debuggers: gdb, valgrind
 *
 *    27/8/21
 */

#include <iostream>
#include <locale>
#include <memory>
#include <fstream>

#include "Engine.h"
#include "Game.h"
#include "HiScore.h"

#define MAP_PATH "resources/Map.txt"

int main(int argc, char **argv) {

	setlocale(LC_ALL, "");

	std::unique_ptr<Game> game;

	if(argc == 1) {
		game = std::make_unique<Game>(Game(MAP_PATH));
	} else if(argc == 2) {
		game = std::make_unique<Game>(Game(argv[1])); // For taking the map
	} else if(argc == 3) {
		game = std::make_unique<Game>(Game(argv[1], argv[2])); // For taking the map
	}

	game->start(); // Start game

	return 0;
}
