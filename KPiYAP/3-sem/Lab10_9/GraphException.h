#ifndef LAB10_9_GRAPH_EXCEPTION_H
#define LAB10_9_GRAPH_EXCEPTION_H

class GraphException : public std::exception
{
private:
    std::string errorMessage;
public:
    explicit GraphException(std::string err_msg) : errorMessage(std::move(err_msg)) {}

    const char *what()
    { return errorMessage.c_str(); }
};


#endif //LAB10_9_GRAPH_EXCEPTION_H
