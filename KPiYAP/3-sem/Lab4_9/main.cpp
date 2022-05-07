// Lab 4.9

#include "Text.h"
#include <iostream>

void addText(std::vector<Text>& texts)
{
    int pages;
    int lines;
    int letters;

    do {
        std::cout << "Enter amount of Pages.\n";
        Input::getValue(pages);
    } while (pages < 1);

    do {
        std::cout << "Enter amount of Lines.\n";
        Input::getValue(lines);
    } while (lines < 1);

    do {
        std::cout << "Enter amount of Letters.\n";
        Input::getValue(letters);
    } while (letters < 1);


    texts.emplace_back(pages, lines, letters);
    std::cin >> texts.at(texts.size() - 1);
}
void deleteText(std::vector<Text>& texts)
{
    int index;
    if (!texts.empty())
    {
        do {
            std::cout << "Enter index of Text to delete.\n";
            Input::getValue(index);
        } while (index < 0 || index > texts.size() - 1);
        try
        {
            auto iter = texts.cbegin();
            texts.erase(iter + index);
        }
        catch (...)
        {
            std::cout << "Wrong Index!\n";
        }
    } else
        std::cout << "No Info!\n";
}
void showTexts(const std::vector<Text>& texts)
{
    if (!texts.empty())
        for (int i = 0; i < texts.size(); i++)
        {
            std::cout << "Text " << i + 1 << ".\n";
            std::cout << texts[i];
        }
    else
        std::cout << "No Info!\n";
}
void editText(std::vector<Text>& texts)
{
    int index;
    if (!texts.empty())
    {
        do {
            std::cout << "Enter index of Text to edit.\n";
            Input::getValue(index);
        } while (index < 0 || index > texts.size() - 1);

        try
        {
            edit(texts.at(index));
        }
        catch (...)
        {
            std::cout << "Wrong Index!\n";
        }
    } else
        std::cout << "No Info!\n";
}
void sortText(std::vector<Text>& texts)
{
    int index;
    if (!texts.empty())
    {
        do {
            std::cout << "Enter index of Text to sort.\n";
            Input::getValue(index);
        } while (index < 0 || index > texts.size() - 1);

        try
        {
            texts.at(index).Sort();
        }
        catch (...)
        {
            std::cout << "Wrong Index!\n";
        }
    } else
        std::cout << "No Info!\n";
}
int menu()
{
    int option;
    do {
        std::cout << "Choose option.\n"
                     "1 - Add Text\n"
                     "2 - Show all Texts\n"
                     "3 - Edit Text\n"
                     "4 - Delete Text\n"
                     "5 - Sort Text\n\n"
                     "6 - Exit\n";
        Input::getValue(option);
    } while (option < 1 || option > 6);
    
    return option;
}


int main()
{
    Text t1(1, 3, 30);
    Text t2(1, 3, 30);
    Text t3(1, 3, 30);
    std::cin >> t1;
    std::cin >> t2;
    std::cin >> t3;
//    weightMatrix(t1, t2);

    std::vector<Text> texts;
    texts.push_back(t1);
    texts.push_back(t2);
    texts.push_back(t3);

//    Text result = getMaxText(texts);
    Text result = getAverageText(texts);
//    std::cout << "Result of sorting by Lowest:\n";
    std::cout << result << std::endl;

    return 0;

//    std::vector<Text> texts;
//    while (true)
//    {
//        switch (menu())
//        {
//            case 1:
//                addText(texts);
//                break;
//            case 2:
//                showTexts(texts);
//                break;
//            case 3:
//                editText(texts);
//                break;
//            case 4:
//                deleteText(texts);
//                break;
//            case 5:
//                sortText(texts);
//                break;
//            case 6:
//                return 0;
//        }
//    }

}

