#include "binaryTree.h"


int main()
{
    // для отладочки
    setbuf(stdout, NULL);

    struct treeNode *root = NULL;
    // structural variable with information
    struct info information;

    char key[STRING_SIZE];

    while (1)
    {
        switch (menu())
        {
            case 1:
                do {
                    information = getInfo();
                    root = insert(root, information);

                    rewind(stdin);
                    printf("Type 'y' to continue: ");
                } while ( getchar() == 'y' );
                break;
            case 2:
                if (root)
                    outputMenu(root);
                else puts("Tree is empty!");
                break;
            case 3:
                if (root)
                {
                    printf("Enter key to delete:");
                    rewind(stdin);
                    fgets(key, STRING_SIZE, stdin);

                    deleteNodeByKey(&root, key);
                }
                break;
            case 4:
                if (root)
                    do {
                        printf("Enter key to search:");
                        rewind(stdin);
                        fgets(key, STRING_SIZE, stdin);

                        printNode( search(root, key) );

                        rewind(stdin);
                        printf("Type 'y' to search another item: ");
                    } while (getchar() == 'y' );
                else
                    puts("Tree is empty!");
                break;
            case 5:
                destroyTree(&root);
                return 0;
        }
    }
}
