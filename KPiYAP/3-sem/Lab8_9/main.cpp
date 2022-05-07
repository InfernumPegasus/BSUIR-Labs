#include <vector>
#include "TStack.h"

int menu()
{
    int option;
    std::cout << "Choose option:\n" <<
              "1 - Push\n"
              "2 - Pop\n"
              "3 - Print\n"
              "4 - Find first element\n"
              "5 - Erase\n"
              "6 - Delete copies\n"
              "7 - TODO replace elements\n"
              "8 - Sort stack\n"
              "9 - Exit\n\n";
    do {
        Input::getValue(option);
    } while (option < 1 || option > 9);
    return option;
}

void terminate()
{
    std::cout << "terminate() function called\n";
    abort();
}

void unexpected()
{
    std::cout << "unexpected() function called\n" << "std::exception will be thrown!\n";
    throw std::exception();
}

void exception_handler()
{
    std::cout << "exception_handler() function called\n";
}

void get_exception() noexcept(false)
{
    std::cout << "Throwing exception!\n";
    throw std::bad_exception();
}

int main()
{
//    std::vector<TStack<int, 5>> vector;
//
//    TStack<int, 5> s1;
//    TStack<int, 3> s2;
//    TStack<int, 5> s3;
//    TStack<int, 5> s4;
//
//    add(vector, s1);

//    // установка нового обработчика исключений для new [[unused]]
//    std::set_new_handler(exception_handler);

    std::set_terminate(terminate);

    std::set_unexpected(unexpected);

/*    TStack<int, 3> stack;
//    int value;
//
//    while (true)
//    {
//        switch (menu())
//        {
//            case 1:
//                std::cout << "Enter Int to push!\n";
//                Input::getValue(value);
//                try {
//                    stack.push(value);
//                }
//                catch (StackException& exception) {
//                    std::cout << exception.what() << std::endl;
//                }
//                break;
//            case 2:
//                try {
//                    stack.pop();
//                }
//                catch (StackException& exception) {
//                    std::cout << exception.what() << std::endl;
//                }
//                break;
//            case 3:
//                try {
//                    std::cout << stack;
//                }
//                catch (StackException& exception) {
//                    std::cout << exception.what() << std::endl;
//                }
//                break;
//            case 4:
//                if (!stack.isEmpty())
//                {
//                    std::cout << "Enter Int to find!\n";
//                    Input::getValue(value);
//                    try {
//                        stack.find_first(value);
//                    }
//                    catch (StackException& exception) {
//                        std::cout << exception.what() << std::endl;
//                    }
//                } else
//                    std::cout << "Stack is Empty!\n";
//                break;
//            case 5:
//                try {
//                    stack.erase();
//                }
//                catch (StackException& exception) {
//                    std::cout << exception.what() << std::endl;
//                }
//                break;
//            case 6:
//                //delete_copies
//                break;
//            case 7:
//                //replace_elements
//                break;
//            case 8:
//                //sort
//                break;
//            case 9:
//                return 0;
//        }
//    }
*/
    /*
    "6 -  Delete copies"        DONE
    "7 -  replace elements"     DONE
    "8 -  Sort stack\n"         DONE
     */

    TStack<int, 7> stack1;

    stack1.push(1);
    stack1.push(2);
    stack1.push(3);
    stack1.push(3);
    stack1.push(2);
    stack1.push(5);
    stack1.push(4);

    stack1.print();

    stack1.insert(0, 999);

    stack1.sort(false);

    stack1.print();

    stack1.sort(true);

//    stack1.removeCopies();

    stack1.print();



//    try {
//        int v4 = 6;
//        stack1.find_first(v4);
//    }
//    catch (StackException& exception) {
//        std::cout << exception.what();
//    }

//    TStack<int, 3> stack2;
//    try {
//        stack2.erase();
//    }
//    catch (StackException& exception) {
//        std::cout << exception.what();
//    }
//
//    try {
//        Output::printBool(stack1 == stack2);
//    }
//    catch (StackException& exception) {
//        std::cout << exception.what();
//    }

    return 0;
}