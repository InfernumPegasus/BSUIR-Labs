template<class T>
void Graph<T>::add(const T &value, const vector<Node *> &neighbors)
{
    if (has_value(value))
        throw GraphException("Graph already has node with value " + to_string(value) + "\n");

    Node *node = new Node(value, neighbors);

    nodes.push_back(node);
}

template<class T>
void Graph<T>::add(const T &value, Graph::Node *neighbor)
{
    if (has_value(value))
        throw GraphException("Graph already has node with value " + to_string(value) + "\n");

    Node *node = new Node(value, neighbor);

    nodes.push_back(node);
}

template<class T>
void Graph<T>::print_nodes()
{
    if (!nodes.empty())
    {
        for (size_t i = 0; i < nodes.size(); i++)
        {
            cout << "Showing [" << i << "] node:" << endl;
            cout << "Node with value : " << nodes.at(i)->get_value() << endl;

            cout << "\tNeighbors of " << nodes.at(i)->_value << " :" << endl;
            for (size_t j = 0; j < nodes[i]->_neighbors.size(); j++)
            {
                if (nodes[i]->_neighbors[j] != nullptr)
                    cout << "\tValue : " << (nodes[i]->_neighbors[j]->get_value()) << endl;
            }
            cout << endl;
        }
    } else {
        throw GraphException("Graph is Empty!\n");
    }

}

template<class T>
bool Graph<T>::has_value(const T &toFind)
{
    for (size_t i = 0; i < nodes.size(); i++)
    {
        T tempValue = nodes.at(i)->get_value();

        if (tempValue == toFind)
            return true;

        for (size_t j = 0; j < nodes[i]->_neighbors.size(); j++)
            if (nodes[i]->_neighbors[j]->get_value() == toFind)
                return true;
    }
    return false;
}

template<class T>
pair<size_t, size_t> Graph<T>::get_indexes_of(const T &toFind)
{
    pair<size_t, size_t> indexes(nodes.size() + 1, nodes.size() + 1);

    for (size_t i = 0; i < nodes.size(); i++)
    {
        T tempValue = nodes.at(i)->get_value();

        if (tempValue == toFind)
            return make_pair(i, 0);

        for (size_t j = 0; j < nodes[i]->_neighbors.size(); j++)
            if (nodes[i]->_neighbors[j]->get_value() == toFind)
                return make_pair(i, j);
    }

    return indexes;
}

template<class T>
void Graph<T>::remove(const T &toRemove)
{
    if (nodes.empty())
        throw GraphException("Graph is Empty!\n");

    for (size_t i = 0; i < nodes.size(); i++)
    {
        T tempValue = nodes.at(i)->get_value();

        if (tempValue == toRemove)
        {
            nodes.erase(nodes.begin() + i);
            std::cout << "Node " << toRemove << " deleted with index " << i << ".\n";
        }

        for (size_t j = 0; j < nodes[i]->_neighbors.size(); j++)
            if (nodes[i]->_neighbors[j]->get_value() == toRemove)
            {
                nodes[i]->_neighbors.erase(nodes[i]->_neighbors.begin() + j);
                std::cout << "Node " << toRemove << " deleted by coordinates [" << i << "][" << j << "].\n";
            }
    }
}


