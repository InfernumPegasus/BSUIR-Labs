#include "Airport.h"

std::string get_file_name(bool is_format_required = true)
{
    std::string filename;
    std::cout << "Enter filename!" << std::endl;
    filename = Input::getStringNoDigs();

    if (is_format_required)
    {
        std::string format;
        int option;
        do {
            std::cout << "Choose format:" << std::endl <<
                         "1 - .txt" << std::endl <<
                         "2 - .bin" << std::endl;
            Input::getValue(option);
        } while (option < 1 || option > 2);
        option == 1 ? format = ".txt" : format = ".bin";
        filename += format;
    }

    return filename;
}

int menu()
{
    int option;
    std::cout << "1 - Add new Airport\n"
                 "2 - Save text file in text mode\n"
                 "3 - Load from text file in text mode\n"
                 "4 - Save file in binary mode\n"
                 "5 - Load from file in binary mode\n"
                 "6 - Show records\n"
                 "7 - Show current records\n"
                 "8 - Delete record from file\n"
                 "9 - Find value in bin file\n"
                 "10 - Find value in text file\n"
                 "11 - Delete current records\n"
                 "12 - (ADD) Delete copies from file\n"
                 "13 - (ADD) Find value in range\n"
                 "14 - (ADD) Sort records by parameter in file\n"
                 "15 - Exit\n\n";
    do {
        std::cout << "Choose option:" << std::endl;
        Input::getValue(option);
    } while (option < 0 || option > 15);

    return option;
}

void add(std::vector<Airport> &vec)
{
    int v1, v2, v3, v4;

    std::cout << "Enter class:\n";
    Input::getValue(v1);

    std::cout << "Enter length:\n";
    Input::getValue(v2);

    std::cout << "Enter width:\n";
    Input::getValue(v3);

    std::cout << "Enter max weight:\n";
    Input::getValue(v4);

    vec.emplace_back(v1, v2, v3, v4);
}

void save_text_in_txt(std::vector<Airport> &vec)
{
    if (vec.empty())
        throw UserException("Vector is Empty!");

    auto filename = get_file_name(false);
    filename += ".txt";

    std::ofstream ofs;

    ofs.open(filename, std::ios::binary);

    if (!ofs.is_open())
        throw UserException("Error with opening!");

    for (auto & i : vec)
    {
        ofs << i;
    }

    ofs.close();
}

void load_text_in_txt(std::vector<Airport> &vec)
{
    auto filename = get_file_name(false);
    filename += ".txt";

    std::ifstream ifs;

    ifs.open(filename, std::ios::binary);

    if (!ifs.is_open())
        throw UserException("Error with opening!");

    std::cout << "Do you want to delete all current records?" << std::endl;
    bool isDeletingNeeded = Input::getBoolean();

    if (isDeletingNeeded)
    {
        std::cout << "Clearing all records!" << std::endl;
        vec.clear();
    }

    while (!ifs.eof())
    {
        int v1, v2, v3, v4;

        ifs >> v1;

        if (ifs.eof())
            break;

        ifs >> v2;
        ifs >> v3;
        ifs >> v4;

        vec.emplace_back(v1, v2, v3, v4);
    }

    ifs.close();
}

void save_in_bin(std::vector<Airport> &vec)
{
    auto filename = get_file_name(true);

    try {
        Airport::save_file_in_bin(filename, vec);
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl;
    }

}

void load_in_bin(std::vector<Airport> &vec)
{
    auto filename = get_file_name(true);

    try {
        Airport::load_file_in_bin(filename, vec);
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl;
    }
}

void show_in_file(const std::vector<Airport> &vec)
{
    auto filename = get_file_name(true);

    bool isInDirectOrder = Input::getBoolean();

    try {
        Airport::show_file(filename, isInDirectOrder);
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl;
    }
}

void show_current_records(const std::vector<Airport> &vec)
{
    for (const auto & item : vec)
    {
        item.info();
        std::cout << std::endl;
    }
}

void delete_in_file()
{
    auto filename = get_file_name(true);

    try {
        Airport::delete_from_file(filename);
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl;
    }
}

void delete_copies(std::vector<Airport> &airports)
{
    if (airports.empty())
        throw UserException("No info in file!");

    auto filename = get_file_name(true);

    std::cout << "Before:" << std::endl;
    for (const auto& item : airports)
        item.info();

    std::cout << std::endl << std::endl;

    try {
        Airport::delete_copies(filename);
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl;
    }

    airports.clear();

    Airport::load_file_in_bin(filename, airports);

    std::cout << "After:" << std::endl;
    for (const auto& item : airports)
        item.info();
}

void print_found_in_bin()
{
    auto filename = get_file_name(true);

    std::vector<Airport> res;

    try {
        res = Airport::find_in_file_in_bin(filename);
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl;
        return;
    }

    std::cout << "Found records:" << std::endl;
    for (const auto & item : res)
        item.info();
}

void print_found_in_txt()
{
    auto filename = get_file_name(true);

    std::vector<Airport> res;

    try {
        res = Airport::find_in_file_in_txt(filename);
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl;
        return;
    }

    std::cout << "Found records:" << std::endl;
    for (const auto & item : res)
        item.info();
}

void print_found_in_range()
{
    auto filename = get_file_name(true);

    std::vector<Airport> res;

    try {
        res = Airport::find_in_range_in_bin(filename);
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl;
        return;
    }

    std::cout << "Found records:" << std::endl;
    for (const auto & item : res)
        item.info();
}

void sort_in_file_in_bin()
{
    std::vector<Airport> airports;
    auto filename = get_file_name(true);
    int index;

    do {
        std::cout << "1 - Class\n"
                     "2 - Length\n"
                     "3 - Width\n"
                     "4 - Max weight\n"
                     "Choose option:\n";
        Input::getValue(index);
    } while (index < 1 || index > 4);

    std::cout << "Sorting by ";
    switch (index)
    {
        case 1:
            std::cout << "Class" << std::endl;
            break;
        case 2:
            std::cout << "Length" << std::endl;
            break;
        case 3:
            std::cout << "Width" << std::endl;
            break;
        case 4:
            std::cout << "Max weight" << std::endl;
            break;
        default:
            break;
    }

    try {
        Airport::load_file_in_bin(filename, airports);
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl;
    }

    try {
        Airport::sort_by_parameter(airports, index);
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl;
    }

    try {
        Airport::save_file_in_bin(filename, airports);
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl;
    }

}

// Загрузка по объектам
// 1 сортировка объектов по любому полю               (DONE)
// 2 поиск по диапозону                               (DONE)
// 3 удаление дубликатов (полное совпадение объектов) (DONE)


int main()
{
    Airport airport1(1, 3000, 100, 140);
    Airport airport2(111,111,111,111);
    Airport airport3(12, 54, 4333, 65);
    Airport airport4(23, 22, 4, 6);
    Airport airport5(12, 2, 23, 65);

    std::vector<Airport> airports;
    airports.push_back(airport1);
//    airports.push_back(airport1);
//    airports.push_back(airport1);
    airports.push_back(airport2);
//    airports.push_back(airport2);
//    airports.push_back(airport2);
    airports.push_back(airport3);
    airports.push_back(airport4);
    airports.push_back(airport5);

    while (true)
    {
        switch (menu())
        {
            case 1:
                try {
                    add(airports);
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
                break;
            case 2:
                try {
                    save_text_in_txt(airports);
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
                break;
            case 3:
                try {
                    load_text_in_txt(airports);
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
                break;
            case 4:
                try {
                    save_in_bin(airports);
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
                break;
            case 5:
                try {
                    load_in_bin(airports);
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
                break;
            case 6:
                try {
                    show_in_file(airports);
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
                break;
            case 7:
                try {
                    show_current_records(airports);
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
                break;
            case 8:
                try {
                    delete_in_file();
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
                break;
            case 9:
                try {
                    print_found_in_bin();
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
                break;
            case 10:
                try {
                    print_found_in_txt();
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
                break;
            case 11:
                try {
                    airports.clear();
                    std::cout << "Records deleted!" << std::endl;
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
                break;
            case 12:
                try {
                    delete_copies(airports);
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
            case 13:
                try {
                    print_found_in_range();
                }
                catch (UserException &exception) {
                    std::cout << exception.what() << std::endl;
                }
                break;
            case 14:
                sort_in_file_in_bin();
                break;
            case 15:
                return 0;
        }
    }

}
