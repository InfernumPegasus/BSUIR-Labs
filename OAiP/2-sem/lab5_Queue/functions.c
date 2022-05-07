//
// Created by USER on 09.05.2021.
//

#include "functions.h"

void getFileName(char* string, int len)     // the function gets file name
{
    char buf; // buffer for each character

    while((buf = (char)getchar()) != '\n' && --len > 0)
        *string++ = buf;

    *string = '\0';
}

int fileNameCheck(const char *string)     // the function checks the correctness of the entered file name
{
    int i = 0;

    while(string[i])
    {
        if(string[i] == '/' || string[i] == '~' || string[i] == '|' || string[i] == '\\' || string[i] == '<' || string[i] == '>' ||
           string[i] == '{' || string[i] == '}' || string[i] == '[' || string[i] == ']' || string[i] == '%' || string[i] == '&' ||
           string[i] == '+' || string[i] == '?' || string[i] == '"' || string[i] == '*' || string[i] == ':' || string[i] == '#' ||
           string[i] == '@' || string[i] == '=' || string[i] == '$')
        {
            return 1;
        }

        i++;
    }
    return 0;
}

char* getName()     // function to get animalName
{
    rewind(stdin);

    char* str = NULL;
    char c;
    size_t i;

    for (i = 0; (c = (char)getchar()) != '\n'; ++i)
    {
        // character check
        if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z'))
        {
            printf("Invalid input, try again!\n");
            return NULL;
        }
        // reallocation check
        if (!(str = (char*)realloc(str, i + 2)))
        {
            printf("Not enough memory!\n");
            return NULL;
        }
        // converts letters to the correct case
        (c >= 'A' && c <= 'Z') ? str[i] = (char)(c + ('a' - 'A')) : (str[i] = c);
    }
    if (str) str[i] = '\0';
    rewind(stdin);
    return str;
}

bool getDate(char str[8])   // boolean function for inputting and checking date format
{
    size_t i;
    rewind(stdin);
    for (i = 0; i < 8; i++)
        if (((i == 2 || i == 4) && (char)getchar() != '.') || ((str[i] = (char)getchar()) < '0' || str[i] > '9'))
        {
            puts("Invalid input, try again!");
            return false;
        }
    if ((str[2] == '1' && str[3] > '2') || str[2] > '1' || str[0] > '3' || (str[0] == '3' && str[1] > 1))
    {
        puts("Such date don't exist!");
        rewind(stdin);
        return false;
    }
    return true;
}

int inputCheck()
{
    int readCount;
    char c;
    int number;

    while(1)
    {
        rewind(stdin);
        readCount = scanf("%d%c", &number, &c);
        if (readCount == 2 && c == '\n')
        {
            break;
        }
        printf("Wrong input! Try again: ");
    }
    return number;
}

int menu()  // UI-menu
{
    int option;

    printf("Choose option:"
           "\n1 - Create/add info to queue"
           "\n2 - Output queue"
           "\n3 - Delete element from queue"
           "\n4 - Find info in queue"
           "\n5 - Save info in file"
           "\n6 - Load from file"
           "\n7 - Exit"
           "\n> ");

    option = inputCheck();
    while(option < 1 || option > 7)
    {
        printf("Wrong input! Try again: ");
        option = inputCheck();
    }

    return option;
}

void putString(char* string, int length)
{
    printf("%s", string);
    for(unsigned long i = strlen(string); i < length; i++)
    {
        printf("%c", ' ');
    }
}

void putHead()
{
    puts("\n");
    puts("-------------------------------------------------");
    puts("| AMOUNT |       NAME       |    DATE    | LIFE |");
    puts("-------------------------------------------------");
}