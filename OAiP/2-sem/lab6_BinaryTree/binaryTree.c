//
// Created by USER on 27.05.2021.
//

#include "binaryTree.h"

struct info getInfo()   // getting info from user
{
    // structural variable which contains info
    struct info temp;

    rewind(stdin);
    puts("Enter origin:");
    fgets(temp.origin, STRING_SIZE, stdin);

    rewind(stdin);
    puts("Enter translation:");
    fgets(temp.translation, STRING_SIZE, stdin);

    rewind(stdin);
    return temp;
}

void recursivePreOrder(struct treeNode *root)    // tree pre order output
{
    if (root != NULL)
    {
        printf("Origin: %s", root->record.origin);
        printf("Translation: %s\n", root->record.translation);

        recursivePreOrder(root->left);
        recursivePreOrder(root->right);
    }
}

void showTreeUsingStack(struct treeNode *root)
{
    struct Stack
    {
        struct treeNode *tree;
        struct Stack *next;
    };
    struct Stack *stackElement, *treeElement = NULL;

    if (!root)
    {
        puts("Tree is empty!");
        return;
    }

    /*
     * вывод первого узла в дереве
     * с последующим выводом других
     */

    // признак движения влево по ветви дерева
    bool traffic = true;

    stackElement = (struct Stack *)calloc(1, sizeof(struct Stack));
    // в стек заносится указатель на корень дерева для прохода по поддеревьям
    stackElement->tree = root;
    // указатель на следующий элемент
    stackElement->next = treeElement;

    printf("Origin: %s", root->record.origin);
    printf("Translation: %s\n", root->record.translation);

    while (stackElement || root -> right)
    {
        while(root -> left || root -> right)
        {
            // цикл пока не лист дерева(лист - указатель, у которого нет потомков)
            if(traffic && root -> left)
            {
                // переход на узел слево
                root = root -> left;
            } else if(root -> right)
            {
                // переход на узел справо
                root = root -> right;
            }
            traffic = true;

            if(root->left && root->right)
            {
                // в буфер заносим вершину стека
                treeElement = stackElement;
                stackElement = (struct Stack *)calloc(1, sizeof(struct Stack));
                // указатель на найденый узел дерева
                stackElement->tree = root;
                // указатель из вершины стека на предыдущий
                stackElement->next = treeElement;
            }
            printf("Origin: %s", root->record.origin);
            printf("Translation: %s\n", root->record.translation);
        }

        if (stackElement)
        {
            root = stackElement->tree;
            treeElement = stackElement->next;
            free(stackElement);
        }

        stackElement = treeElement;

        if (root -> left && root -> right)
            traffic = false;
        else
            break;
    }
}

void printNode(struct treeNode *node)   // меню вывода узла
{
    if (node)
    {
        printf("Origin: %s\n", node->record.origin);
        printf("Translation: %s\n\n", node->record.translation);
        return;
    }
    puts("NULL!");
}

struct treeNode *search (struct treeNode *node, char key[])     // поиск узла по ключу
{
    if (node != NULL)
    {
        int conditional = strcmp(key, node->record.origin);

        if ( conditional == 0 )
            return node;

        if (conditional < 0)
            return search(node->left, key);

        if (conditional > 0)
            return search(node->right, key);
    }
    return NULL;
}

int height (struct treeNode *node)
{
    if (node != NULL)
    {
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);

        return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    }
    return 0;
}

int difference (struct treeNode *node)
{
    return height(node->left) - height(node->right);
}

struct treeNode *leftLeftRotation (struct treeNode *node)
{
    struct treeNode *temp;
    temp = node->left;
    node->left = temp->right;
    temp->right = node;

    return temp;
}

struct treeNode *rightRightRotation (struct treeNode *node)
{
    struct treeNode *temp;
    temp = node->right;
    node->right = temp->left;
    temp->left = node;

    return temp;
}

struct treeNode *leftRightRotation (struct treeNode *node)
{
    struct treeNode *temp;
    temp = node->left;
    node->left = rightRightRotation(temp);

    return leftLeftRotation(node);
}

struct treeNode *rightLeftRotation (struct treeNode *node)
{
    struct treeNode *temp;
    temp = node->right;
    node->right = leftLeftRotation(temp);

    return rightRightRotation(node);
}

struct treeNode *balance (struct treeNode *temp)    // балансировка дерева
{
    int balanceFactor = difference(temp);

    if (balanceFactor > 1)
    {
        if ( difference(temp->left) > 0 )
            temp = leftLeftRotation(temp);
        else
            temp = leftRightRotation(temp);
    }
    else if (balanceFactor < -1)
    {
        if ( difference(temp->right) > 0 )
            temp = rightLeftRotation(temp);
        else
            temp = rightRightRotation(temp);
    }
    return temp;
}

void deleteNodeByKey(struct treeNode **Node, char Data[])    // удаление элемента по ключу
{
    if ( (*Node) != NULL )
    {
        // если узел не NULL и строки совпадают
        if ( (strcmp((*Node)->record.origin, Data)) == 0 )
        {
            // создаем промежуточный указатель
            // и запоминаем текущий узел
            struct treeNode *ptr = (*Node);

            // если у узла нет потомков
            // то присваеваем ему NULL
            if ( (*Node)->left == NULL && (*Node)->right == NULL )
                (*Node) = NULL;
                // если у узла нет лишь левого потомка
                // переносим "вершину" на правого потомка удаляемого узла
            else if ((*Node)->left == NULL)
                (*Node) = ptr->right;
                // аналогично с правым потомком
            else if ((*Node)->right == NULL)
                (*Node) = ptr->left;
                // если удаляемый узел - корень
            else {
                // переносим "вершину" на правого потомка
                (*Node) = ptr->right;
                // создаем адресную переменную
                struct treeNode **reserve;
                // передаем адрес вершины созданной переменной
                reserve = Node;
                // до тех пор, пока не достигнем конца левой части дерева
                // присваиваем адреса левых потомков адресной переменной
                while (*reserve != NULL)
                    reserve = &((*reserve)->left);
                // после чего присваиваем указатель на самого левого потомка
                (*reserve) = ptr->left;
            }
            free(ptr);
            deleteNodeByKey(Node,Data);
        }
            // если узел не найден
            // то рекурсивно проходим по дереву
            // пока не будет достигнут NULL
            // или не найден узел
        else {
            deleteNodeByKey(&((*Node)->left),Data);
            deleteNodeByKey(&((*Node)->right),Data);
        }
    }
}

void destroyTree (struct treeNode **root)   // полная очистка дерева
{
    if( (*root) == NULL )
        return;
    if((*root)->left)
        destroyTree(&((*root)->left));
    else if((*root)->right)
        destroyTree(&((*root)->right));
    free(*root);
    *root = NULL;
}

struct treeNode *insert (struct treeNode *node, struct info information)
{
    int conditional;
    if (node == NULL)
    {
        // выделяем память под узел
        node = (struct treeNode *)malloc(sizeof(struct treeNode));

        // копируем информацию в созданный узел
        strcpy(node->record.origin, information.origin);
        strcpy(node->record.translation, information.translation);

        node->left = node->right = NULL;    // инициализируем потомков NULL
    }
    else
    {
        if ( (conditional = strcmp(information.origin, node->record.origin)) < 0 )
        {
            // вызов функции вставки в левом потомке узла
            // с последующей балансировкой
            node->left = insert(node->left, information);
            node = balance(node);
        }
        if (conditional > 0)
        {
            // вызов функции вставки в правом потомке узла
            // с последующей балансировкой
            node->right = insert(node->right, information);
            node = balance(node);
        }
    }
    return node;
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
        if (readCount == 2 && c == '\n' && number > 0)
        {
            break;
        }
        printf("Wrong input! Try again: ");
    }

    return number;
}

void outputMenu(struct treeNode *root)      // меню для выбора способа вывода дерева
{
    int option;
    printf("Choose method of output\n"
           "1 - Recursive\n"
           "2 - Non-recursive\n"
           "3 - Exit\n"
           ">");
    while ( (option = inputCheck()) > 3 )
        printf("Wrong input! Try again: ");


    rewind(stdin);
    switch (option)
    {
        case 1:
            recursivePreOrder(root);
            break;
        case 2:
            showTreeUsingStack(root);
            break;
        default:
            return;
    }
}

int menu()  // UI-menu
{
    int option;

    printf("Choose option:"
           "\n1 - Create/add info to tree"
           "\n2 - Output tree"
           "\n3 - Delete element from tree"
           "\n4 - Find info in tree"
           "\n5 - Exit"
           "\n> ");

    while ( (option = inputCheck()) > 5 )
        printf("Wrong input! Try again: ");

    return option;
}

