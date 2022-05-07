#ifndef LAB4_9_TEXT_H
#define LAB4_9_TEXT_H

#include <vector>
#include "UserInput.h"

class Text
{
private:
    char ***m_text = nullptr;
    size_t m_pages{};
    size_t m_lines{};
    size_t m_letters{};

    // метод для выделения памяти
    void AllocMemory()
    {
        m_text =  new char**[m_pages];
        for (int i = 0; i < m_pages; ++i)
        {
            m_text[i] = new char*[m_lines];
            for (int j = 0; j < m_lines; ++j)
                m_text[i][j] = new char[m_letters];
        }
    }

    // set-методы
    void setPages(size_t value);
    void setLines(size_t value);
    void setLetters(size_t value);
public:
    // конструктор по умолчанию
    Text()
    {
        m_pages   = 1;
        m_lines   = 1;
        m_letters = 20;
        m_text    = nullptr;

        AllocMemory();
    }

    // конструктор инициализации
    Text(const size_t pages, const size_t lines, const size_t letters, char ***text = nullptr)
    : m_pages(pages), m_lines(lines), m_letters(letters), m_text(text)
    {
        AllocMemory();
    }

    // конструктор копирования
    Text(const Text& supply)
    : m_pages(supply.m_pages), m_lines(supply.m_lines), m_letters(supply.m_letters)
    {
        AllocMemory();

        for (int i = 0; i < m_pages; ++i)
            for (int j = 0; j < m_lines; ++j)
                std::strcpy(m_text[i][j], supply.m_text[i][j]);
    }

    // деструктор
    ~Text()
    {
        std::cout << "Destructor in Text!\n";
        for (int i = 0; i < m_pages; ++i)
        {
            for (int j = 0; j < m_lines; ++j)
                delete m_text[i][j];
            delete m_text[i];
        }
        delete [] m_text;
    }

    // функции для сортировки по возрастанию и убыванию
    static int str_cmp_inc (const void * a, const void * b);
    static int str_cmp_dec (const void * a, const void * b);

    // перегрузкки операторов для работы с памятью
    void* operator new(size_t size);
    void* operator new[](size_t size);
    void operator delete(void* ptr);
    void operator delete[](void* ptr);

    // get-методы
    size_t getPages() const;
    size_t getLines() const;
    size_t getLetters() const;

    // перегрузка операторов ввода и вывода
    friend std::ostream& operator<< (std::ostream &out, const Text& text);
    friend std::istream& operator>> (std::istream &in, Text& t);

    // методы для работы с экземплярами класса
    friend void edit(Text& t);
    void Sort();

    //1) отсортировать массив матрицы от меньшего к большему (по кодам)
    friend void weightMatrix(const Text& t1, const Text& t2);

    //2) вывод одной матрицы, каждый строка в кадой матрице которой является отсортированной по строкам в массиве матриц
    static bool isEqualSize(const Text& t);
    friend Text getMaxText(const std::vector<Text>& source);

    friend Text getAverageText(const std::vector<Text>& source);
};



#endif //LAB4_9_TEXT_H
