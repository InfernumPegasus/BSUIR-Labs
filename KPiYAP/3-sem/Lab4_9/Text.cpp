#include "Text.h"

std::ostream& operator<< (std::ostream &out, const Text& text)
{
    for (int i = 0; i < text.m_pages; ++i)
    {
        for (int j = 0; j < text.m_lines; ++j)
            out << "Text[" << i + 1 << "][" << j + 1 << "] = " << text.m_text[i][j] << std::endl;
        std::cout << "\n";
    }

    return out;
}

std::istream &operator>>(std::istream &in, Text &text)
{
    for (int i = 0; i < text.m_pages; ++i)
    {
        for (int j = 0; j < text.m_lines; ++j)
        {
            std::cout << "Text[" << i + 1 << "][" << j + 1<< "]:\n";
            text.m_text[i][j] = Input::getStringNoDigs(text.m_letters);
        }
        std::cout << "\n";
    }

    return in;
}

void edit(Text& t)
{
    size_t pages;
    size_t lines;
    size_t letters;

    do {
        std::cout << "Enter amount of Pages:\n";
        Input::getValue(pages);
    } while (pages < 1);

    do {
        std::cout << "Enter amount of Lines:\n";
        Input::getValue(lines);
    } while (lines < 1);

    do {
        std::cout << "Enter amount of Letters:\n";
        Input::getValue(letters);
    } while (letters < 1);

    t.setPages(pages);
    t.setLines(lines);
    t.setLetters(letters);

    t.AllocMemory();

    std::cin >> t;
}

void Text::operator delete[](void *ptr)
{
    std::cout << "Using delete[] operator\n";
    ::operator delete[](ptr);
}
void Text::operator delete(void *ptr)
{
    std::cout << "Using delete operator\n";
    ::operator delete(ptr);
}

void *Text::operator new[](size_t size)
{
    std::cout << "Using new[] operator\n";
    void *ptr = ::operator new[](size);
    return ptr;
}
void *Text::operator new(size_t size)
{
    std::cout << "Using new operator\n";
    void *ptr = ::operator new(size);
    return ptr;
}

int Text::str_cmp_inc(const void *a, const void *b)
{ return strcmp(*(char**)a,*(char**)b); }
int Text::str_cmp_dec(const void *a, const void *b)
{ return -strcmp(*(char**)a,*(char**)b); }


void Text::Sort()
{
    std::cout << "Sorting...\n";
    for (int i = 0; i < m_pages; i++)
        qsort(m_text[i], m_lines, sizeof(m_text[i]), str_cmp_inc);
}

size_t Text::getPages() const
{ return m_pages; }
size_t Text::getLines() const
{ return m_lines; }
size_t Text::getLetters() const
{ return m_letters; }

void Text::setPages(size_t value)
{ m_pages = value; }
void Text::setLines(size_t value)
{ m_lines = value; }
void Text::setLetters(size_t value)
{ m_letters = value; }

// ДОП 1 вывод большей матрицы по весу суммарных ASCII-кодов
void weightMatrix(const Text &t1, const Text &t2)
{
    int w1 = 0;
    int w2 = 0;
    for (int i = 0; i < t1.getPages(); i++)
        for (int j = 0; j < t1.getLines(); j++)
        {
            int k = 0;
            while (t1.m_text[i][j][k++] != '\0')
                w1 += (int)t1.m_text[i][j][k];
        }

    for (int i = 0; i < t2.getPages(); i++)
        for (int j = 0; j < t2.getLines(); j++)
        {
            int k = 0;
            while (t2.m_text[i][j][k++] != '\0')
                w2 += (int)t2.m_text[i][j][k];
        }
    std::cout << "Weight of 1: " << w1
              << std::endl << "Weight of 2: " << w2
              << std::endl;
}

// ДОП 2 матрица, состоящая из строк из двух матриц
bool Text::isEqualSize(const Text &t)
{
    int lengths[t.getPages()][t.getLines()];
    for (int i = 0; i < t.getPages(); i++)
        for (int j = 0; j < t.getLines(); j++)
            lengths[i][j] = strlen(t.m_text[i][j]);

    int tSize = lengths[0][0];
    for (int i = 0; i < t.getPages(); i++)
        for (int j = 0; j < t.getLines(); j++)
            if (lengths[i][j] != tSize)
                return false;
    return true;
}



Text getMaxText(const std::vector<Text> &source)
{
    for (const auto & amount : source)
    {
        bool isAllLengthsEqual = Text::isEqualSize(amount);
        if (!isAllLengthsEqual)
        {
            std::cout << "Incorrect lengths!\n";
            exit(1);
        }
    }

    int pages   = source[0].getPages();
    int lines   = source[0].getLines();
    int letters = source[0].getLetters();

    std::cout << "Before sort:\n";
    for (const auto & i : source)
        std::cout << i << std::endl;

    // проходим по количеству матриц
    for (const auto & amount : source)
    {
        // проходим по каждой странице каждой матрицы
        for (int pages = 0; pages < amount.getPages(); pages++)
            qsort(amount.m_text[pages], amount.getLines(), sizeof(amount.m_text[pages]), Text::str_cmp_inc);
    }

    std::cout << "After sort:\n";
    for (const auto & i : source)
        std::cout << i << std::endl;

    // создание итогового текста
    Text result(pages, lines, letters);

    for (int amount = 0; amount < pages; amount++)
        for (int j = 0; j < lines; j++)
            result.m_text[amount][j] = source.at(amount).m_text[amount][j];

    return result;
}

Text getAverageText(const std::vector<Text> &source)
{
    // проверка на равенство их голосов
    for (const auto & amount : source)
    {
        bool isAllLengthsEqual = Text::isEqualSize(amount);
        if (!isAllLengthsEqual)
        {
            std::cout << "Incorrect lengths!\n";
            exit(1);
        }
    }

    // кол-во матриц
    int pages   = source[0].getPages();

    // характеристики каждой матрицы
    int lines   = source[0].getLines();
    int letters = source[0].getLetters();

    // создание итогового текста
    Text result(pages, lines, letters);

    // цикл по страницам каждого текста (по кол-ву матриц)
    for (int page = 0; page < pages; page++)
    {
        // цикл по кол-ву линий
        for (int line = 0; line < lines; line++)
        {
            // проходим по каждой строке и отбираем нужный символ
            for (int letter = 0; letter < letters; letter++)
            {
                int code = 0;
                char c;
                // идем по каждому элементу-символу вектора
                for (const auto & amount : source)
                {
                    code += (int)amount.m_text[page][line][letter];
                }
                c = (char)(code / source.size());

                result.m_text[page][line][letter] = c;
            }
        }
    }

    return result;
}










