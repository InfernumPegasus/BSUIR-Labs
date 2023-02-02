#ifndef MYGIT_COMMIT_H
#define MYGIT_COMMIT_H

#include "File.h"
#include <nlohmann/json.hpp>
#include <algorithm>

// TODO добавить хэш-сумму и уникальный ID коммита
class Commit {
public:
    Commit(const std::vector<std::string> &files, std::string message) :
            fileNames_(files),
            message_(std::move(message)) {}

public:
    [[nodiscard]]
    constexpr auto FileNames() const -> std::vector<std::string> {
        return fileNames_;
    }

    [[nodiscard]]
    constexpr auto Message() const -> std::string {
        return message_;
    }

    [[nodiscard]]
    constexpr auto Contains(std::string_view filename) const -> bool {
        return std::find(fileNames_.begin(), fileNames_.end(), filename) != fileNames_.end();
    }

//    [[nodiscard]]
//    constexpr auto Find(std::string_view filename) const {
//        return std::find(fileNames_.begin(), fileNames_.end(), filename);
//    }

public:
    [[nodiscard]]
    auto ToJson() const -> nlohmann::json {
        nlohmann::json j;
        j["file_names"] = fileNames_;
        j["message"] = message_;
        return j;
    }

private:
    std::vector<std::string> fileNames_;
    std::string message_;
};


#endif //MYGIT_COMMIT_H
