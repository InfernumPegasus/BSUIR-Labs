#include <iostream>
#include <fstream>
#include "File.h"

std::string File::Name() const {
    return name_;
}

int64_t File::Hash() const {
    return hash_;
}

FileStatus File::Status() const {
    return status_;
}

std::string File::ModificationTime() const {
    return modificationTime_;
}

constexpr bool File::operator==(const File &rhs) const {
    return name_ == rhs.name_ && hash_ == rhs.hash_ && modificationTime_ == rhs.modificationTime_;
}

bool File::operator<(const File &rhs) const {
    return hash_ < rhs.hash_;
}

auto File::ToJson() const -> nlohmann::json {
    nlohmann::json j;
    j["name"] = name_;
    j["hash"] = hash_;
    j["status"] = status_;
    j["mod"] = modificationTime_;
    return j;
}

File File::FromJson(nlohmann::json json) {
    std::string name = json["name"];
    int64_t hash = json["hash"];
    FileStatus status = json["status"];
    std::string modTime = json["mod"];
    return {name, hash, modTime, status};
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

int64_t File::CalculateHash(std::string_view filename) {
    auto content = LoadContent(filename);
    auto res = static_cast<int64_t>(content.size());
    std::string file = filename.data();
    for (auto &c: content) {
        res ^= c + static_cast<int64_t>(std::hash<std::string>{}(file)) +
               0x9e3779b9 + (res << 6) + (res >> 2);
    }
    return res;
}

std::string File::LastWriteTimeString(std::string_view filename) {
    using std::to_string;

    auto ftime = std::filesystem::last_write_time(filename);

    static int64_t sfclock_diff = [] {
        auto diff = int64_t(std::chrono::system_clock::now().time_since_epoch() /
                            std::chrono::milliseconds(1)) -
        int64_t(decltype(ftime)::clock::now().time_since_epoch() / std::chrono::milliseconds(1));
        return (diff + (diff < 0 ? -500 : 500)) / 1000; // Round to the nearest second.
    }();

    std::time_t cftime = std::chrono::system_clock::to_time_t(
            std::chrono::time_point<std::chrono::system_clock>(
                    std::chrono::seconds(ftime.time_since_epoch() / std::chrono::seconds(1) + sfclock_diff)
            )
    );
    auto resTime = std::localtime(&cftime);
    std::string time = to_string(resTime->tm_year) + " " +
                       to_string(resTime->tm_mon) + " " +
                       to_string(resTime->tm_mday) + " " +
                       to_string(resTime->tm_hour) + " " +
                       to_string(resTime->tm_min) + " " +
                       to_string(resTime->tm_sec);
    return time;
}