#include "Graph.h"

int main()
{
    Graph<int> int_graph;

    auto *node1 = new Graph<int>::Node(1);
    auto *node2 = new Graph<int>::Node(2);
    auto *node3 = new Graph<int>::Node(3);
    auto *node4 = new Graph<int>::Node(4);
    auto *node5 = new Graph<int>::Node(5);

    auto *node6 = new Graph<int>::Node(6);
    auto *node7 = new Graph<int>::Node(7);
    auto *node8 = new Graph<int>::Node(8);
    auto *node9 = new Graph<int>::Node(9);

    vector<Graph<int>::Node *> nodes1;
    nodes1.push_back(node1);
    nodes1.push_back(node2);
    nodes1.push_back(node3);
    nodes1.push_back(node4);
    nodes1.push_back(node5);

    vector<Graph<int>::Node *> nodes2;
    nodes2.push_back(node2);
    nodes2.push_back(node1);
    nodes2.push_back(node3);

    // Одинаковые элементы
    nodes2.push_back(node3);
    nodes2.push_back(node4);
    nodes2.push_back(node5);

    vector<Graph<int>::Node *> nodes3;
    nodes3.push_back(node6);
    nodes3.push_back(node8);
    nodes3.push_back(node7);
    nodes3.push_back(node9);
    nodes3.push_back(node4);

    try {
        int_graph.add(999, nodes1);

        int_graph.add(888, nodes2);

        int_graph.add(777, nodes3);

        int_graph.add(666, nodes3);
    }
    catch (GraphException &exception) {
        cout << exception.what() << endl;
    }

    cout << "Before:\n";
    int_graph.print_nodes();

    int_graph.sort();

    cout << "After:\n";
    int_graph.print_nodes();

//    auto indexes = int_graph.get_indexes_of(99999);
//    cout << indexes.first << " " << indexes.second << endl;


//    int value = 5;
//    cout << "Before deleting " << to_string(value) << endl;
//
//    try {
//        int_graph.print_nodes();
//    }
//    catch (GraphException &exception) {
//        cout << exception.what() << endl;
//    }
//
//    try {
//        int_graph.remove(value);
//    }
//    catch (GraphException &exception) {
//        cout << exception.what() << endl;
//    }
//
//
//    cout << "After deleting " << to_string(value) << endl;
//    try {
//        int_graph.print_nodes();
//    }
//    catch (GraphException &exception) {
//        cout << exception.what() << endl;
//    }


    return 0;
}


