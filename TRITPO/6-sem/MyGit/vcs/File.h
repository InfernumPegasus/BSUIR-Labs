#ifndef MYGIT_FILE_H
#define MYGIT_FILE_H

#include <fstream>
#include <utility>
#include <vector>
#include "FileStatus.h"
#include <nlohmann/json.hpp>

using namespace std::string_literals;

// TODO добавить валидацию пути
class File {
public:
    explicit File(std::string name) : name_(std::move(name)) {}

    File(std::string_view name, const std::vector<char> &content) :
            name_(name),
            content_(content) {}

    File(const File &rhs) = default;

public:
    [[nodiscard]]
    constexpr auto Name() const -> std::string {
        return name_;
    }

    [[nodiscard]]
    constexpr auto Content() const -> std::vector<char> {
        return content_;
    }

    auto operator==(const File &other) -> bool {
        return name_ == other.name_ && content_ == other.content_;
    }

    bool LoadContent() {
        std::ifstream ifs(name_);
        if (ifs.is_open()) {
            char c;
            while (ifs.good()) {
                ifs.read(&c, sizeof(char));
                content_.push_back(c);
            }
        }
    }

public:
    [[nodiscard]]
    auto ToJson() const -> nlohmann::json {
        nlohmann::json j;
        j["name"] = name_;
        j["content"] = content_;
        return j;
    }

    static File FromJson(nlohmann::json json) {
        std::string name = json["name"];
        std::vector<char> content = json["content"];
        return {name, content};
    }

    [[nodiscard]]
    constexpr auto Hash() const -> size_t {
        size_t res{0};
        for (auto &c : Content()) {
            res ^= std::hash<size_t>{}(c);
        }
        return res;
    }

private:
    std::vector<char> content_;

    std::string name_;
};


#endif //MYGIT_FILE_H
