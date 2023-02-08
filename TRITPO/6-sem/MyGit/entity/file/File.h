#ifndef MYGIT_FILE_H
#define MYGIT_FILE_H

#include "FileStatus.h"
#include <nlohmann/json.hpp>
#include <fstream>

class File {
public:
    explicit File(const std::string &name) :
            name_(name),
            hash_(CalculateHash(name)) {}

    File(const File &rhs) = default;
    File &operator=(const File &rhs) = default;

    File(File &&rhs) noexcept = default;
    File &operator=(File &&rhs) noexcept = default;

private:
    File(std::string name, size_t hash, FileStatus status) :
            name_(std::move(name)),
            hash_(hash),
            status_(status) {}

public:
    [[nodiscard]] constexpr auto Name() const -> std::string {
        return name_;
    }

    [[nodiscard]] constexpr auto Hash() const -> size_t {
        return hash_;
    }

    [[nodiscard]] constexpr auto Status() const -> FileStatus {
        return status_;
    }

    constexpr bool operator==(const File &rhs) const {
        return name_ == rhs.name_ && hash_ == rhs.hash_ && status_ == rhs.status_;
    }

    bool operator<(const File &rhs) const {
        return hash_ < rhs.hash_;
    }

    static auto LoadContent(std::string_view filename) -> std::vector<char> {
        std::vector<char> content;
        std::ifstream ifs(filename.data());
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
    [[nodiscard]] auto ToJson() const -> nlohmann::json {
        nlohmann::json j;
        j["name"] = name_;
        j["hash"] = hash_;
        j["status"] = status_;
        return j;
    }

    static File FromJson(nlohmann::json json) {
        std::string name = json["name"];
        size_t hash = json["hash"];
        FileStatus status = json["status"];
        return {name, hash, status};
    }


    static auto CalculateHash(std::string_view filename) -> size_t {
        auto content = LoadContent(filename);
        size_t res = content.size();
        std::string file = filename.data();
        for (auto &c: content) {
            res ^= static_cast<size_t>(c) +
                   std::hash<std::string>{}(file) +
                   0x9e3779b9 + (res << 6) + (res >> 2);
        }
        return res;
    }

private:
    size_t hash_;

    std::string name_;

    FileStatus status_{FileStatus::Unknown};
};


#endif //MYGIT_FILE_H
