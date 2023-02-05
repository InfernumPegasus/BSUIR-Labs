#ifndef MYGIT_COMMIT_H
#define MYGIT_COMMIT_H

#include "File.h"
#include <nlohmann/json.hpp>
#include <algorithm>
#include <set>

// TODO добавить хэш-сумму и уникальный ID коммита
class Commit {
public:
    Commit(const std::set<std::string> &files, std::string message) :
            fileNames_(files),
            message_(std::move(message)) {
    }

//    Commit(const std::vector<std::string> &files, std::string_view message) :
//            fileNames_(files),
//            message_(message) {}

public:
    [[nodiscard]]
    auto FileNames() const -> std::set<std::string> {
        return fileNames_;
    }

    [[nodiscard]]
    constexpr auto Message() const -> std::string {
        return message_;
    }

    [[nodiscard]]
    auto Contains(std::string_view filename) const -> bool {
        return fileNames_.contains(filename.data());
    }

public:
    [[nodiscard]]
    auto ToJson() const -> nlohmann::json {
        nlohmann::json j;
        j["file_names"] = fileNames_;
        j["message"] = message_;
        return j;
    }

    static Commit FromJson(nlohmann::json json) {
        std::set<std::string> filenames = json["file_names"];
        std::string message = json["message"];
        return {filenames, message};
    }

private:
    std::set<std::string> fileNames_;
    std::string message_;
};


#endif //MYGIT_COMMIT_H
