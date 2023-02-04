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
    explicit File(std::string name) : name_(std::move(name)) {
//        if (std::filesystem::exists(name_)) {
//            status_ = FileStatus::Exists;
//        }
    }

    File(const File &rhs) = default;

//    File(File &&rhs) noexcept : File(rhs.Name()) {}

public:
    [[nodiscard]]
    constexpr auto Name() const -> std::string {
        return name_;
    }

//    [[nodiscard]]
//    constexpr auto Status() const -> FileStatus {
//        return status_;
//    }

    auto operator==(const File &other) -> bool {
        return name_ == other.name_; //&& status_ == other.status_;
    }

//public:
//    void SetStatus(FileStatus status) {
//        status_ = status;
//    }

public:
    [[nodiscard]]
    auto ToJson() const -> nlohmann::json {
        nlohmann::json j;
        j["name"] = name_;
//        j["status"] = status_;
        return j;
    }

private:
//    std::vector<char> content_;
    std::string name_;

//    FileStatus status_ {FileStatus::Unknown};
};


#endif //MYGIT_FILE_H
