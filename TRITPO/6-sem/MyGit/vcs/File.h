#ifndef MYGIT_FILE_H
#define MYGIT_FILE_H

#include <fstream>
#include <vector>

using namespace std::string_literals;

// TODO добавить валидацию пути
class File {
public:
    explicit File(std::string name) : name_(std::move(name)) {
        std::ifstream ifs(name_);
        if (!ifs) {
            throw std::invalid_argument("File '"s
                                                .append(name_)
                                                .append("' cannot be opened."));
        }
        char c;
        while (ifs.good()) {
            ifs.read(&c, sizeof(char));
            content_.push_back(c);
        }
    }

    File(const File &rhs) = default;

    File(File &&rhs) noexcept :
            name_(std::move(rhs.name_)),
            content_(std::move(rhs.content_)) {}

public:
    constexpr auto operator==(const File &rhs) -> bool {
        return content_ == rhs.content_;
    }

public:
    constexpr auto Name() -> std::string {
        return name_;
    }

    constexpr auto Content() -> std::vector<char> {
        return content_;
    }

private:
    std::vector<char> content_;

    std::string name_;
};


#endif //MYGIT_FILE_H
