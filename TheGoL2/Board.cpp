#include "Board.h"

Board::Board() {
    parse_rules("#R B3/S23");
    count_b = 0;
    count_s = 0;
    for (int i = 0; i < 9; i++) {
        if (forbirth[i] == 1)
            count_b++;
        if (forsurvival[i] == 1)
            count_s++;
    }
    width = 115;
    height = 15;
    number_iteration = 0;
    field = new char* [width];
    for (int i = 0; i < width; i++)
        field[i] = new char[height];
    str_div = get_str();
}

Board::Board(int widthn, int heightn, std::string rule) {
    parse_rules(rule);
    count_b = 0;
    count_s = 0;
    for (int i = 0; i < 9; i++) {
        if (forbirth[i] == 1)
            count_b++;
        if (forsurvival[i] == 1)
            count_s++;
    }
    width = abs(widthn);
    number_iteration = 0;
    height = abs(heightn);
    field = new char* [width];
    for (int i = 0; i < width; i++)
        field[i] = new char[height];
    str_div = get_str();
}

void Board::parse_rules(std::string rule) {
    int i = 4;
    while (rule[i] != '/') {
        forbirth[rule[i++] - '0'] = 1;
    }
    i += 2;
    while (rule[i] != '\0') {
        forsurvival[rule[i++] - '0'] = 1;
    }
}

void Board::fill_glider() {
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            field[i][j] = ' ';
    field[1][0] = '*';
    field[4][0] = '*';
    field[0][1] = '*';
    field[0][2] = '*';
    field[4][2] = '*';
    field[0][3] = '*';
    field[1][3] = '*';
    field[2][3] = '*';
    field[3][3] = '*';
}

void Board::fill_from_file(std::list<std::pair<int, int>> life_list) {
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            field[i][j] = ' ';
    for (auto& elem : life_list)
        field[abs(elem.first)][abs(elem.second)] = '*';
}

void Board::show_board() {
    std::cout << str_div << std::endl;
    for (int i = 0; i < height; i++) {
        std::cout << "|";
        for (int j = 0; j < width; j++) {
            std::cout << field[j][i];
        }
        std::cout << "|";
        std::cout << std::endl;
    }
    std::cout << str_div << std::endl;
}

void Board::step() {
    Board prev;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            prev.field[j][i] = field[j][i];

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int count_neighbors = 0;
            count_neighbors += prev.field[get_x(i - 1)][get_y(j - 1)] == '*' ? 1 : 0;
            count_neighbors += prev.field[get_x(i)][get_y(j - 1)] == '*' ? 1 : 0;
            count_neighbors += prev.field[get_x(i + 1)][get_y(j - 1)] == '*' ? 1 : 0;
            count_neighbors += prev.field[get_x(i - 1)][get_y(j)] == '*' ? 1 : 0;
            count_neighbors += prev.field[get_x(i + 1)][get_y(j)] == '*' ? 1 : 0;
            count_neighbors += prev.field[get_x(i - 1)][get_y(j + 1)] == '*' ? 1 : 0;
            count_neighbors += prev.field[get_x(i)][get_y(j + 1)] == '*' ? 1 : 0;
            count_neighbors += prev.field[get_x(i + 1)][get_y(j + 1)] == '*' ? 1 : 0;

            for (int k = 0; k < count_b; k++) {
                for (int l = 0; l < 9; l++) {
                    if (forbirth[l] == 1) {
                        if (prev.field[i][j] == ' ' && count_neighbors == l)
                            field[i][j] = '*';
                        else
                            field[i][j] = ' ';
                    }
                }
            }
            for (int k = 0; k < count_s; k++) {
                for (int l = 0; l < 9; l++) {
                    if (prev.field[i][j] == '*' && forsurvival[l] == 0 && count_neighbors == l)
                        field[i][j] = ' ';
                    else if (prev.field[i][j] == '*' && forsurvival[l] == 1 && count_neighbors == l) {
                        field[i][j] = '*';
                        break;
                    }
                }
            }
        }
    }
    number_iteration++;
}

void Board::save_universe(std::string outpath, std::string name, std::string rules) {
    std::ofstream out(outpath);
    out << "#Life 1.06" << '\n' << name << '\n' << rules << '\n' << width << " " << height;
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            if (field[i][j] == '*')
                out << '\n' << i << " " << j;
}

std::string Board::get_str() {
    std::string str;
    for (int j = 0; j < width + 2; j++) {
        if (j == 0 || j == width + 1)
            str.append("+");
        else
            str.append("-");
    }
    return str;
}

int Board::get_x(int i) {
    return (width + i) % width;
}

int Board::get_y(int i) {
    return (height + i) % height;
}

int Board::get_width() {
    return width;
}

int Board::get_height() {
    return height;
}

int Board::get_number_loops() {
    return number_iteration;
}
