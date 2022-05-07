#ifndef LAB9_9_AIRPORT_H
#define LAB9_9_AIRPORT_H

#include "../lib/UserInput.h"
#include "../lib/UserOutput.h"

#include <vector>
#include <fstream>
#include <algorithm>

class Airport
{
private:
    int m_airport_class;
    int m_length;
    int m_width;
    int m_max_weight;
public:
    Airport(int airport_class, int length, int width, int max_weight)
    : m_airport_class(airport_class), m_length(length), m_width(width), m_max_weight(max_weight)
    {}

    Airport(const Airport& supply)
    : Airport(supply.m_airport_class, supply.m_length, supply.m_width, supply.m_max_weight)
    {}

    Airport()
    : Airport(0, 0, 0, 0)
    {}


    /*
     * Метод для просмотра информации
     */

    void info() const;

    /*
     * Вспомогательный метод для удаления элемента из вектора
     */

    template<class T>
    static void resize_vector(std::vector<T> &vec, size_t index)
    {
        if (vec.empty())
        {
            std::cout << "Vector is Empty!\n";
            throw std::exception();
        }
        if (index < 0 || index >= vec.size())
        {
            std::cout << "Wrong index!\n";
            throw std::out_of_range("Out of range Exception!\n");
        }

        std::rotate(vec.begin() + index , vec.begin() + index + 1, vec.end());
        vec.pop_back();
    }


    /*
     * Операторы для работы с текстовыми файлами
     */

    friend std::ostream &operator<<(std::ostream &out, const Airport& value);

    friend std::istream &operator>>(std::istream &in, Airport& value);


    /*
     * Методы для работы с файлами
     */

    static void save_text_in_bin(const std::string &filename, const std::vector<Airport> &airports);

    static void load_text_in_bin(const std::string &filename, std::vector<Airport> &airports);


    /*
     * Вывод записей в прямом и обратном порядке без считывания всех записей
     * (используя временнуюю переменную типа Airport и/или стек)
     */

    static void show_file(const std::string &filename, bool is_direct_order = true);


    /*
     * Методы для работы с бинарными файлами
     */

    static void save_file_in_bin(const std::string &filename, const std::vector<Airport> &airports);

    static void load_file_in_bin(const std::string &filename, std::vector<Airport> &airports);


    /**
     * @brief Перегруженный оператор сравнения
     * @param airport Экземпляр для сравнения
     * @return true если поля равны, в противном случае false
     */

    bool operator==(const Airport &airport) const
    {
        return m_airport_class == airport.m_airport_class && m_length == airport.m_length
               && m_max_weight == airport.m_max_weight    && m_width  == airport.m_width;
    }

    /*
     * Метод удаления записи из файла
     * Доп 3. Удаление при полном совпадении
     */

    static void delete_from_file(std::string &filename);

    static void delete_copies(std::string &filename);


    /*
     * Метод для поиска записи из файла по параметру
     * Доп 2. Поиск в диапазоне
     */

    static std::vector<Airport> find_in_file_in_bin(const std::string &filename);

    static std::vector<Airport> find_in_file_in_txt(const std::string &filename);

    static std::vector<Airport> find_in_range_in_bin(const std::string &filename);


    /**
     * Метод сортировки записей по параметру
     * @param airports Вектор экземпляров класса Airport
     * @param par_index Индекс параметра для сортировки
     */
    static void sort_by_parameter(std::vector<Airport> &airports, int par_index);
};


/**
 * @brief Класс пользовательского типа исключений
 */
class UserException : public std::exception
{
private:
    std::string errorMessage;
public:
    explicit UserException(std::string err_msg) : errorMessage(std::move(err_msg)) {}

    const char *what()
    { return errorMessage.c_str(); }
};


#endif //LAB9_9_AIRPORT_H
