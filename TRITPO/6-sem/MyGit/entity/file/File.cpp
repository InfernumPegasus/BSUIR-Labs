#include <fstream>
#include "File.h"

auto File::Name() const -> std::string {
    return name_;
}

auto File::Hash() const -> std::size_t {
    return hash_;
}

constexpr auto File::Status() const -> FileStatus {
    return status_;
}

constexpr bool File::operator==(const File &rhs) const {
    return name_ == rhs.name_ && hash_ == rhs.hash_ && status_ == rhs.status_;
}

bool File::operator<(const File &rhs) const {
    return hash_ < rhs.hash_;
}

auto File::ToJson() const -> nlohmann::json {
    nlohmann::json j;
    j["name"] = name_;
    j["hash"] = hash_;
    j["status"] = status_;
    return j;
}

File File::FromJson(nlohmann::json json) {
    std::string name = json["name"];
    size_t hash = json["hash"];
    FileStatus status = json["status"];
    return {name, hash, status};
}

auto File::LoadContent(std::string_view filename) -> std::vector<char> {
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

auto File::CalculateHash(std::string_view filename) -> size_t {
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

