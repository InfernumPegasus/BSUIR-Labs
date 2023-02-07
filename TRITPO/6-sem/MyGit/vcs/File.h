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
    explicit File(std::string name) :
            name_(std::move(name)),
            hash_(Hash()) {}

    File(const File &rhs) = default;

    File &operator=(const File &rhs) = default;

    File(File &&rhs) noexcept:
            name_(std::move(rhs.name_)),
            hash_(rhs.hash_) {}

    File &operator=(File &&rhs) noexcept = default;

private:
    File(std::string_view name, size_t hash) :
            name_(name),
            hash_(hash) {}

public:
    [[nodiscard]]
    constexpr auto Name() const -> std::string {
        return name_;
    }

    bool operator<(const File &rhs) const {
        return hash_ < rhs.hash_;
    }

    [[nodiscard]]
    auto LoadContent() const -> std::vector<char> {
        std::vector<char> content;
        std::ifstream ifs(name_);
        if (ifs.is_open()) {
            char c;
            while (ifs.good()) {
                ifs.read(&c, sizeof(char));
                content.push_back(c);
            }
        }
        return content;
    }

public:
    [[nodiscard]]
    auto ToJson() const -> nlohmann::json {
        nlohmann::json j;
        j["name"] = name_;
        j["hash"] = hash_;
        return j;
    }

    static File FromJson(nlohmann::json json) {
        std::string name = json["name"];
        size_t hash = json["hash"];
        return {name, hash};
    }

    [[nodiscard]]
    auto Hash() const -> size_t {
        auto content = LoadContent();
        size_t res = content.size();
        for (auto &c: content) {
            res ^= static_cast<size_t>(c) + 0x9e3779b9 + (res << 6) + (res >> 2);
        }
        return res;
    }

private:
    size_t hash_;

    std::string name_;
};


#endif //MYGIT_FILE_H
