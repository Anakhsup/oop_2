#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <fstream>
#include <thread>
#include <list>
#include <conio.h>
#include "Board.h"

class Ruler {
public:
	Ruler();
	int read_argv(int argc, char* argv[]);
	std::list<std::pair<int, int>> read_file();
	void start_online_game(Board* board);
	void start_offline_game(Board* board);
	void start_game_from_file(Board* board);
	void start();
	void func_reader(Board* board);
private:
	unsigned int iterations = 100;
	int mode = 1;
	int width, height;
	std::string inpath;
	std::string outpath;
	std::string rules;
	std::string name;
	std::list<std::pair<int, int>> life_list;
};

