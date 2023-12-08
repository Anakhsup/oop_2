#include "Rules.h"

Ruler::Ruler() { //правила
    iterations = 0;
    mode = 1;
    inpath = "";
    outpath = "";
    rules = "";
    name = "";
    width = 115;
    height = 15;
}

int Ruler::read_argv(int argc, char* argv[]) { //если есть аргументы
    if (argc <= 2) {
        if (argc == 2) { //есть только файл
            inpath = argv[1];
            mode = 0;
            return 0;  
        }
        else {
            if (argc == 1) { //ничего нет
                mode = 1;
                return 1;  
            }
            else { //некорректный ввод
                std::cout << "Недопустимые аргументы: необходимо ввести имя файла" << std::endl;
                mode = -1;
                return -1;  
            }
        }

    }
    else {
        inpath = argv[1];
        int i = 2;
        while (argv[i] != nullptr) {
            if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--iterations") == 0) { //итерации
                i++;
                iterations = std::stoi(argv[i]);
            }
            if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) { //запись в файл
                i++;
                outpath = argv[i];  
                if (outpath.empty()) { //error
                    std::cout << "Недопустимые аргументы: аргумент после -o должен быть строковым" << std::endl;
                    mode = -1;
                    return -1; 
                }
            }
            i++;
        }
        //есть файл, число итераций, выходной файл
        mode = 2;
        return 2;  
    }
}

std::list<std::pair<int, int>> Ruler::read_file() {  //считывание с файла
    std::ifstream input(inpath);
    std::string format;
    int x, y;  //координаты живых клеток
    std::list<std::pair<int, int>> life_list;  //лист с живыми клетками
    if (input.is_open()) {
        std::getline(input, format);
        if (format == "#Life 1.06") {  //формат верный
            std::getline(input, name);
            std::getline(input, rules);
            input >> width >> height;  //ширина, высота
            while (input >> x >> y)
                life_list.push_back(std::make_pair(x, y));
        }
        else
            std::cout << "Некорректный формат Вселенной" << std::endl;
    }
    input.close();
    return life_list;
}

void Ruler::start_online_game(Board* board) { //старт, если ничего нет
    bool flag = false;
    while (!flag) {
        board->show_board();
        board->step();
        char key = 0; //ожидание команды с клавиатуры
        if (_kbhit()) {
            key = _getch();
            if (key == '`') { //клавиша для остановки
                flag = true;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20)); //скорость изменения
        system("cls"); //очистка консоли
    }
}

void Ruler::start_offline_game(Board* board) { //старт, если есть всё
    for (unsigned int i = 0; i < iterations; i++)
        board->step();
}

void Ruler::start_game_from_file(Board* board) { //старт, если есть только файл
    bool flag = false;
    while (!flag) {
        board->show_board();
        board->step();
        char key = 0;
        if (_kbhit()) {
            key = _getch();
            if (key == '`') {
                flag = true;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        system("cls");
    }
}

void Ruler::start() {
    switch (mode) {
    case 0: { //есть только файл
        life_list = read_file();
        Board board(width, height, rules);
        board.fill_from_file(life_list);
        start_game_from_file(&board);
        func_reader(&board);
        break;
    }
    case 1: { //ничего нет
        name = "#N Lightweight spaceship";
        rules = "#R B3/S23";
        Board board;
        board.fill_glider();
        start_online_game(&board);
        func_reader(&board);
        break;
    }
    case 2: { //есть все
        life_list = read_file();
        Board board(width, height, rules);
        board.fill_from_file(life_list);
        start_offline_game(&board);
        board.save_universe(outpath, name, rules);
        break;
    }
    case -1: //error
        break;
    default:
        break;
    }
}

enum class Command {
    Help = 104, // 'h'
    Dump = 100, // 'd'
    Exit = 101, // 'e'
    Tick = 116  // 't'
};

void Ruler::func_reader(Board* board) {  // исполнение команд
    bool flag = false;
    while (!flag) {
        std::cout << "Введите команду:\n";
        std::string comand;
        std::getline(std::cin, comand);

        Command command = static_cast<Command>(comand[0]);

        switch (command) {
        case Command::Help:
            std::cout << '\n';
            std::cout << "Список команд:" << '\n';
            std::cout << "help - выводит список команд и их значение" << '\n';
            std::cout << "dump <file_name> - сохраняет Вселенную в формате Life 1.06" << '\n';
            std::cout << "tick <n=1> и t <n=1> - вычисляет состояние Вселенной после n циклов" << '\n';
            std::cout << "  и выводит Вселенную результата с его названием, правилами и номером цикла" << '\n';
            std::cout << "exit - покинуть/завершить игру Жизнь" << '\n';
            std::cout << '\n';
            break;
        case Command::Dump:
            if (comand.length() > 1 && comand[1] == ' ') {
                std::string filename = comand.substr(2);
                board->save_universe(filename, name, rules);
            }
            break;
        case Command::Exit:
            flag = true;
            break;
        case Command::Tick:
            if (comand.length() > 1 && comand[1] == ' ') {
                std::string temp = comand.substr(2);
                std::stringstream ss(temp);
                int loops;
                ss >> loops;
                for (unsigned int i = 0; i < loops; i++)
                    board->step();
                std::cout << name << '\n' << rules << '\n' << board->get_number_loops() << '\n';
                board->show_board();
            }
            else {
                board->step();
                std::cout << name << '\n' << rules << '\n' << board->get_number_loops() << '\n';
                board->show_board();
            }
            break;
        default:
            std::cout << "Некорректная команда. Введите \"help\" для получения списка команд" << '\n';
            break;
        }
    }
}

