#include <iostream>
#include <vector>
#include <algorithm>

#include "GraphException.h"

using namespace std;

template<class T>
class Graph
{
public:
    /**
     * @brief Структура, реализующая каждый узел
     */
    struct Node
    {
        // Значение узла
        T _value;

        // Вектор соседей
        vector<Node *> _neighbors;

        // TODO мб добавить вес ребра
        // size_t _weight;

        bool has_value(const T &toFind)
        {
            for (size_t i = 0; i < _neighbors.size(); i++)
            {
                T tempValue = _neighbors.at(i)->get_value();

                if (tempValue == toFind)
                    return true;

                for (size_t j = 0; j < _neighbors[i]->_neighbors.size(); j++)
                    if (_neighbors[i]->_neighbors[j]->get_value() == toFind)
                        return true;
            }
            return false;
        }

        explicit Node(const T &value, const vector<Node *> &neighbors)
        {
            _value = value;

            for (auto & item : neighbors)
                if (item && !has_value(value) && !has_value(item->_value))
                    _neighbors.push_back(item);
        }

        explicit Node(const T &value, Node *neighbor = nullptr)
        {
            _value = value;

            if (neighbor)
            {
                Node *temp = neighbor;

                if (!has_value(value) && !has_value(neighbor->_value))
                {
                    temp->_neighbors.push_back(neighbor);

                    _neighbors.push_back(temp);
                }


                delete temp;
            }

        }

        bool operator==(const Node &node)
        { return _value == node._value; }

        bool operator!=(const Node &node)
        { return _value != node._value; }

        bool operator>(const Node &node)
        { return _value > node._value; }

        bool operator<(const Node &node)
        { return _value < node._value; }

        T get_value()
        { return _value; }
    };

    /**
     * @brief Метод для добавления узла и его соседей
     * @param value Значение для добавления
     * @param neighbor Вектор указателей на соседей
     */
    void add(const T &value, const vector<Node *> &neighbors);

    /**
     * @brief Метод для добавления узла и его соседей
     * @param value Значение для добавления
     * @param neighbor Указатель на узел соседа
     */
    void add(const T &value, Node *neighbor = nullptr);

    /**
     * @brief Метод для вывода всех элементов графа
     */
    void print_nodes();

    /**
     * @brief Метод для поиска значения в графе
     * @param toFind параметр, который нужно найти
     * @return true если параметр есть в графе, и false если не найден
     */
    bool has_value(const T &toFind);

    /**
     * @brief Метод для поиска значения в графе
     * @param toFind параметр, который нужно найти
     * @return std::pair c индексами если значение найдено, std::pair с -1 и -1, если значение не было найдено
     */
    pair<size_t, size_t> get_indexes_of(const T &toFind);

    /**
     * @brief Метод для удаления узла по значению
     * @param toRemove Значение, которое требуется удалить
     */
    void remove(const T &toRemove);

    bool is_empty()
    { return nodes.empty(); }

    // Доп задание
    void sort()
    {
        if (nodes.empty())
            throw GraphException("Graph is empty!");

        for (size_t i = 0; i < nodes.size() - 1; i++)
        {
            for (size_t j = i + 1; j < nodes.size() ; j++)
            {
                if (nodes[j] > nodes[i])
                    swap(nodes[j], nodes[i]);
            }
        }
    }

private:

    // Вектор узлов, т.е. граф
    vector<Node *> nodes;

};


#include "Graph.inl"