#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <list>
#include <fstream>
#include <sstream>

class Board
{
public:
	Board();
	Board(int width, int height, std::string rule);

	void parse_rules(std::string rule);
	void fill_glider();
	void fill_from_file(std::list<std::pair<int, int>> life_list);
	void show_board();
	void step();
	void save_universe(std::string outpath, std::string name, std::string rules);

	std::string get_str();
	int get_x(int i);
	int get_y(int i);
	int get_width();
	int get_height();
	int get_number_loops();

private:
	std::string str_div;
	int forbirth[9] = { 0 };
	int forsurvival[9] = { 0 };
	int count_b = 0, count_s = 0;
	int width;
	int height;
	int number_iteration;
	char** field;
};

