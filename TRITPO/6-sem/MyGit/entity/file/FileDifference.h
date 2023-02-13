#ifndef MYGIT_FILEDIFFERENCE_H
#define MYGIT_FILEDIFFERENCE_H

#include <vector>
#include <string>

// TODO этот класс должен представлять "разницу" между файлами, представленную массивом строк и их номером в файле
class FileDifference {

private:
    using LineAndIndex = std::pair<std::string, int>;

    std::vector<LineAndIndex> linesAndIndexes_;
};


#endif //MYGIT_FILEDIFFERENCE_H