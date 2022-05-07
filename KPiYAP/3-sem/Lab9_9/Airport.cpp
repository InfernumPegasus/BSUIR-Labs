#include "Airport.h"


/**
 * Метод для вывода информации об объекте
 */
void Airport::info() const
{
    std::cout << "Airport Info:" << std::endl;
    std::cout << "Class:      " << m_airport_class << std::endl;
    std::cout << "Length:     " << m_length << " m" << std::endl;
    std::cout << "Width:      " << m_width << " m" << std::endl;
    std::cout << "Max Weight: " << m_max_weight << " ton" << std::endl << std::endl;
}


/**
 * Перегруженный оператор вывода
 * @param out объект выходного потока std::ostream
 * @param value объект вывода
 * @return объект выходного потока std::ostream
 */
std::ostream &operator<<(std::ostream &out, const Airport &value)
{
    out << value.m_airport_class << std::endl;
    out << value.m_length << std::endl;
    out << value.m_width << std::endl;
    out << value.m_max_weight << std::endl;
    return out;
}


/**
 * Перегруженный оператор ввода
 * @param in объект входного потока std::istream
 * @param value объект ввода
 * @return объект входного потока std::istream
 */
std::istream &operator>>(std::istream &in, Airport &value)
{
    in >> value.m_airport_class;
    in >> value.m_length;
    in >> value.m_width;
    in >> value.m_max_weight;
    return in;
}


/**
 * Метод для сохранения текстовых файлов в бинарном режиме
 * @param filename имя сохраняемого файла
 * @param airports вектор, содержащий записи, подлежащие сохранению
 */
void Airport::save_text_in_bin(const std::string &filename, const std::vector<Airport> &airports)
{
    if (airports.empty())
        throw UserException("Vector is Empty!");

    std::ofstream ofs;

    ofs.open(filename, std::ios::binary);

    if (!ofs.is_open())
        throw UserException("Error with opening!");

    for (auto & item : airports)
    {
        try {
            ofs.write((char*)&item.m_airport_class, sizeof(m_airport_class));
            ofs.write((char*)&item.m_length,        sizeof(m_length));
            ofs.write((char*)&item.m_width,         sizeof(m_width));
            ofs.write((char*)&item.m_max_weight,    sizeof(m_max_weight));
        }
        catch (std::exception &exception) {
            std::cout << "Error with reading!" << std::endl;
            return;
        }
    }
    ofs.close();
}

/**
 * Метод для чтения текстовых файлов в бинарном режиме
 * @param filename имя загружаемого файла
 * @param airports вектор, в который будут записаны все записи
 */
void Airport::load_text_in_bin(const std::string &filename, std::vector<Airport> &airports)
{
    std::ifstream ifs;

    Airport airport;

    ifs.open(filename, std::ios::binary);

    if (!ifs.is_open())
        throw UserException("Error with opening!");

    while (!ifs.eof())
    {
        try {
            ifs.read((char*)&airport.m_airport_class, sizeof(m_airport_class));
            if (ifs.eof())
                break;
            ifs.read((char*)&airport.m_length, sizeof(m_length));
            ifs.read((char*)&airport.m_width, sizeof(m_width));
            ifs.read((char*)&airport.m_max_weight, sizeof(m_max_weight));
        }
        catch (std::exception &exception) {
            std::cout << "Error with reading!" << std::endl;
            return;
        }
        airports.push_back(airport);
    }
    ifs.close();
}


/**
 * Метод для чтения из файла в прямом или обратном порядке
 * @param filename имя файла
 * @param is_direct_order индикатор порядка чтения. true - чтение в прямом порядке, false - в обратном
 */
void Airport::show_file(const std::string &filename, bool is_direct_order)
{
    std::vector<Airport> airports;
    try {
        load_file_in_bin(filename, airports);
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl;
        return;
    }

    if (airports.empty())
        throw UserException("No info in file!");

    if (!is_direct_order)
        std::reverse(airports.begin(), airports.end());

    for (const auto &item : airports)
    {
        item.info();
        std::cout << std::endl;
    }
}


/**
 * Метод для сохранения бинарных файлов
 * @param filename имя сохраняемого файла
 * @param airports вектор, содержащий записи, подлежащие сохранению
 */
void Airport::save_file_in_bin(const std::string &filename, const std::vector<Airport> &airports)
{
    if (airports.empty())
        throw UserException("Vector is Empty!");

    std::ofstream ofs;
    ofs.open(filename, std::ios::binary | std::ios_base::trunc);

    if (!ofs.is_open())
        throw UserException("Error with opening!");

    for (auto & item : airports)
    {
        try {
            ofs.write((char*)&item, sizeof(Airport));
        }
        catch (std::exception &exception) {
            std::cout << exception.what() << std::endl;
            throw std::exception();
        }
    }
    ofs.close();
}


/**
 * Метод для чтения бинарных файлов
 * @param filename имя загружаемого файла
 * @param airports вектор, в который будут записаны все записи
 */
void Airport::load_file_in_bin(const std::string &filename, std::vector<Airport> &airports)
{
    std::ifstream ifs;

    Airport airport;

    ifs.open(filename, std::ios::binary);

    if (!ifs.is_open())
        throw UserException("Error with opening!");

    while (!ifs.eof())
    {
        try {
            ifs.read((char*)&airport, sizeof(Airport));
            if (ifs.eof())
                return;
        }
        catch (std::exception &exception) {
            std::cout << exception.what() << std::endl;
            throw std::exception();
        }
        airports.push_back(airport);
    }
    ifs.close();
}


/**
 * Метод для удаления записей из файлов
 * @param filename имя файла
 */
void Airport::delete_from_file(std::string &filename)
{
    std::vector<Airport> airports;

    int option = 0;
    auto format = filename.substr(filename.size() - 4);
    if (format == ".txt")
        option = 1;
    if (format == ".bin")
        option = 2;

    if (option == 0)
        throw UserException("Wrong file format!");

    if (option == 1)
    {
        try {
            load_text_in_bin(filename, airports);
        }
        catch (UserException &exception) {
            std::cout << exception.what() << std::endl;
        }
    } else {
        try {
            load_file_in_bin(filename, airports);
        }
        catch (UserException &exception) {
            std::cout << exception.what() << std::endl;
        }
    }

    if (airports.empty())
        throw UserException("No info in file!");

    int index;

    if (option == 1)
    {
        if (airports.size() >= 2)
        {
            do {
                std::cout << "Choose record with index from 0 to " << airports.size() - 1  << "\n";
                Input::getValue(index);
            } while (index < 0 || index > airports.size() - 1 );

            std::cout << "\nShowing deleted record!\n\n";
            airports.at(index).info();

            resize_vector(airports, index);
        }
        else
        {
            std::cout << "There is only one record! It will be deleted!\n";
            airports.at(0).info();

            std::ofstream ofs;
            ofs.open(filename, std::ios::binary | std::ios_base::trunc);

            if (!ofs.is_open())
                throw UserException("Error with saving!");
            return;
        }

        try {
            Airport::save_text_in_bin(filename, airports);
        }
        catch (UserException &exception) {
            std::cout << exception.what() << std::endl;
        }
    } else {
        if (airports.size() >= 2)
        {
            do {
                std::cout << "Choose record with index from 0 to " << airports.size() - 1  << "\n";
                Input::getValue(index);
            } while (index < 0 || index > airports.size() - 1 );

            std::cout << "\nShowing deleted record!\n\n";
            airports.at(index).info();

            resize_vector(airports, index);
        }
        else
        {
            std::cout << "There is only one record! It will be deleted!\n";
            airports.at(0).info();

            std::ofstream ofs;
            ofs.open(filename, std::ios::binary | std::ios_base::trunc);

            if (!ofs.is_open())
                throw UserException("Error with saving!");
            return;
        }

        try {
            Airport::save_file_in_bin(filename, airports);
        }
        catch (UserException &exception) {
            std::cout << exception.what() << std::endl;
        }
    }
}

void Airport::delete_copies(std::string &filename)
{
    std::vector<Airport> airports;
    size_t count = 0;

    int option = 0;
    auto format = filename.substr(filename.size() - 4);
    if (format == ".txt")
        option = 1;
    if (format == ".bin")
        option = 2;

    if (option == 0)
        throw UserException("Wrong file format!");

    if (option == 1)
    {
        try {
            load_text_in_bin(filename, airports);
        }
        catch (UserException &exception) {
            std::cout << exception.what() << std::endl;
        }
    } else {
        try {
            load_file_in_bin(filename, airports);
        }
        catch (UserException &exception) {
            std::cout << exception.what() << std::endl;
        }
    }

    if (airports.empty())
        throw UserException("No info in file!");

    for (int i = 0; i < airports.size(); i++)
        for (size_t j = airports.size() - 1; j > i; j--)
            if (airports.at(i) == airports.at(j))
            {
                airports.erase(airports.begin() + j);
                count++;
            }

    if (option == 1)
    {
        try {
            save_text_in_bin(filename, airports);
        }
        catch (UserException &exception) {
            std::cout << exception.what() << std::endl;
            return;
        }
    } else {
        try {
            save_file_in_bin(filename, airports);
        }
        catch (UserException &exception) {
            std::cout << exception.what() << std::endl;
            return;
        }
    }
}


/**
 * Метод для поиска записи в файле
 * @param filename имя файла
 * @return массив найденных записей в случае успешного поиска; @UserException в случае если ничего не было найдено
 */
std::vector<Airport> Airport::find_in_file_in_bin(const std::string &filename)
{
    std::vector<Airport> airports;

    int option{};
    std::string format = filename.substr(filename.size() - 4, filename.size() - 1);
    if (format == ".txt")
        option = 1;
    else if (format == ".bin")
        option = 2;
    else
        throw UserException("Invalid file format!");

    if (option == 1)
    {
        try {
            load_text_in_bin(filename, airports);
        }
        catch (UserException &exception) {
            std::cout << exception.what() << std::endl;
        }
    } else {
        try {
            load_file_in_bin(filename, airports);
        }
        catch (UserException &exception) {
            std::cout << exception.what() << std::endl;
        }
    }

    if (airports.empty())
        throw UserException("No info in file!");

    int toSearch;

    std::vector<Airport> result;

    do {
        std::cout << "Choose parameter to find:\n"
                     "1 - Airport class\n"
                     "2 - Airport length\n"
                     "3 - Airport width\n"
                     "4 - Airport max weight\n\n"
                     "Your option:";
        Input::getValue(option);
    } while (option < 1 || option > 4);

    switch (option)
    {
        case 1:
            std::cout << "Enter Class!\n";
            Input::getValue(toSearch);
            for (auto & airport : airports)
                if (airport.m_airport_class == toSearch)
                    result.push_back(airport);
            break;
        case 2:
            std::cout << "Enter Length!\n";
            Input::getValue(toSearch);
            for (auto & airport : airports)
                if (airport.m_length == toSearch)
                    result.push_back(airport);
            break;
        case 3:
            std::cout << "Enter Width!\n";
            Input::getValue(toSearch);
            for (auto & airport : airports)
                if (airport.m_width == toSearch)
                    result.push_back(airport);
            break;
        case 4:
            std::cout << "Enter Max Weight!\n";
            Input::getValue(toSearch);
            for (auto & airport : airports)
                if (airport.m_max_weight == toSearch)
                    result.push_back(airport);
            break;
        default:
            break;
    }

    if (result.empty())
        throw UserException("Nothing found!");

    return result;
}

std::vector<Airport> Airport::find_in_file_in_txt(const std::string &filename)
{
    std::ifstream ifs;

    Airport temp;

    int option;
    int toSearch;

    std::vector<Airport> result;
    std::vector<Airport> airports;

    ifs.open(filename);

    if (!ifs.is_open())
        throw UserException("Error with opening!\n");

    while (ifs >> temp)
    {
        std::cout << temp << std::endl;

        airports.push_back(temp);
    }

    do {
        std::cout << "Choose parameter to find:\n"
                     "1 - Airport class\n"
                     "2 - Airport length\n"
                     "3 - Airport width\n"
                     "4 - Airport max weight\n\n"
                     "Your option:";
        Input::getValue(option);
    } while (option < 1 || option > 4);

    switch (option)
    {
        case 1:
            std::cout << "Enter Class!\n";
            Input::getValue(toSearch);
            for (auto &airport : airports)
                if (airport.m_airport_class == toSearch)
                    result.push_back(airport);
            break;
        case 2:
            std::cout << "Enter Length!\n";
            Input::getValue(toSearch);
            for (auto &airport : airports)
                if (airport.m_length == toSearch)
                    result.push_back(airport);
            break;
        case 3:
            std::cout << "Enter Width!\n";
            Input::getValue(toSearch);
            for (auto &airport : airports)
                if (airport.m_width == toSearch)
                    result.push_back(airport);
            break;
        case 4:
            std::cout << "Enter Max Weight!\n";
            Input::getValue(toSearch);
            for (auto &airport : airports)
                if (airport.m_max_weight == toSearch)
                    result.push_back(airport);
            break;
        default:
            break;
    }

    return result;
}

std::vector<Airport> Airport::find_in_range_in_bin(const std::string &filename)
{
    std::vector<Airport> airports;
    std::vector<Airport> result;

    int option{};

    std::string format = filename.substr(filename.size() - 4, filename.size() - 1);

    int leftBorder, rightBorder;

    if (format == ".txt")
        option = 1;
    else if (format == ".bin")
        option = 2;
    else
        throw UserException("Invalid file format!");

    if (option == 1)
    {
        try {
            load_text_in_bin(filename, airports);
        }
        catch (UserException &exception) {
            std::cout << exception.what() << std::endl;
        }
    } else {
        try {
            load_file_in_bin(filename, airports);
        }
        catch (UserException &exception) {
            std::cout << exception.what() << std::endl;
        }
    }

    if (airports.empty())
        throw UserException("No info in file!");

    std::cout << "Enter left border:" << std::endl;
    Input::getValue(leftBorder);

    std::cout << "Enter right border:" << std::endl;
    Input::getValue(rightBorder);

    do {
        std::cout << "Choose parameter to find:\n"
                     "1 - Airport class\n"
                     "2 - Airport length\n"
                     "3 - Airport width\n"
                     "4 - Airport max weight\n\n"
                     "Your option:";
        Input::getValue(option);
    } while (option < 1 || option > 4);

    switch (option)
    {
        case 1:
            for (auto & airport : airports)
                if (airport.m_airport_class >= leftBorder && airport.m_airport_class <= rightBorder)
                    result.push_back(airport);
            break;
        case 2:

            for (auto & airport : airports)
                if (airport.m_length >= leftBorder && airport.m_length <= rightBorder)
                    result.push_back(airport);
            break;
        case 3:
            for (auto & airport : airports)
                if (airport.m_width >= leftBorder && airport.m_width <= rightBorder)
                    result.push_back(airport);
            break;
        case 4:
            for (auto & airport : airports)
                if (airport.m_max_weight >= leftBorder && airport.m_max_weight <= rightBorder)
                    result.push_back(airport);
            break;
        default:
            break;
    }

    if (result.empty())
        throw UserException("Nothing found!");

    return result;
}


// ДОП 3

void Airport::sort_by_parameter(std::vector<Airport> &airports, int par_index)
{
    if (airports.empty())
        throw UserException("No records!");

    if (par_index < 1 || par_index > 4)
        throw UserException("Wrong index!");

    // пара вида Значение - Индекс
    std::vector<std::pair<int, size_t>> pairs;

    for (size_t i = 0; i < airports.size(); i++)
    {
        switch(par_index)
        {
            case 1:
                pairs.emplace_back(std::make_pair(airports.at(i).m_airport_class, i));
                break;
            case 2:
                pairs.emplace_back(std::make_pair(airports.at(i).m_length, i));
                break;
            case 3:
                pairs.emplace_back(std::make_pair(airports.at(i).m_width, i));
                break;
            case 4:
                pairs.emplace_back(std::make_pair(airports.at(i).m_max_weight, i));
                break;
            default:
                break;
        }
    }

    std::sort(pairs.begin(), pairs.end());

    std::vector<Airport> newAirports = airports;

    airports.clear();

    for (auto & pair : pairs)
        airports.push_back(newAirports[pair.second]);
}
