#include "BracketsCheck.h"

void BracketsCheck::loadFromTxt()
{
    std::ifstream ifs;
    ifs.open(m_sourceFile,std::ios_base::in);

    if (ifs.is_open())
        std::cout << "File [" << m_sourceFile << "] is loaded\n";
    else {
        std::cerr << "[Wrong filename!] [Occur in " << __PRETTY_FUNCTION__ << "]" << std::endl;
        throw UserException("Wrong filename!");
    }

    while (!ifs.eof())
    {
        std::string singleString;
        std::getline(ifs, singleString, '\n');

        m_brackets.emplace(m_brackets.cend(), singleString);
    }

    ifs.close();
}

void BracketsCheck::check()
{
    std::stack<char> StackDefaultBrackets;
    std::stack<char> StackSquareBrackets;

    for (auto & brackets : m_brackets)
    {
        if (brackets.find_first_of("()[]") == std::string::npos)
        {
            std::cout << " " << brackets << " \t - Has wrong format!\n";
            continue;
        }

        for (char & bracket : brackets)
        {
            if (bracket == '(')
                StackDefaultBrackets.push(bracket);

            if (bracket == ')' && !StackDefaultBrackets.empty())
                StackDefaultBrackets.pop();

            if (bracket == '[')
                StackSquareBrackets.push(bracket);

            if (bracket == ']' && !StackSquareBrackets.empty())
                StackSquareBrackets.pop();
        }

        if (StackDefaultBrackets.empty() && StackSquareBrackets.empty())
        {
            std::cout << " " << brackets << " \t - Correct\n";
            m_correct.emplace(m_correct.cend(), brackets);
        } else {
            std::cout << " " << brackets << " \t - Incorrect\n";
            m_incorrect.emplace(m_incorrect.cend(), brackets);
        }

        while (!StackDefaultBrackets.empty())
            StackDefaultBrackets.pop();

        while (!StackSquareBrackets.empty())
            StackSquareBrackets.pop();
    }
}

void BracketsCheck::saveInTxt()
{
    if (m_correct.empty() && m_incorrect.empty())
    {
        std::cerr << "[No records!] [Occur in " << __PRETTY_FUNCTION__ << "]" << std::endl;
        throw UserException("Empty file!");
    }

    std::ofstream ofs;
    ofs.open(m_resultFile, std::ios_base::out);
    if (ofs.is_open())
        std::cout << "File [" << m_resultFile << "] is loaded\n";
    else {
        std::cerr << "[Wrong filename!] [Occur in " << __PRETTY_FUNCTION__ << "]" << std::endl;
        throw UserException("Wrong filename!");
    }

    ofs << "Correct:\n";
    for (const auto &item : m_correct)
    {
        ofs << item << std::endl;
    }

    ofs << "Incorrect:\n";
    for (const auto &item : m_incorrect)
    {
        ofs << item << std::endl;
    }

    ofs.close();
}