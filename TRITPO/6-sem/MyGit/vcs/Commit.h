#ifndef MYGIT_COMMIT_H
#define MYGIT_COMMIT_H

#include "File.h"
#include <nlohmann/json.hpp>
#include <algorithm>
#include <set>

// TODO добавить хэш-сумму и уникальный ID коммита
class Commit {
public:
    Commit(const std::set<File> &files, std::string message) :
            files_(files),
            message_(std::move(message)) {}

    Commit(const std::set<std::string> &files, std::string message) :
            message_(std::move(message)) {
        std::set<File> filesSet;
        for (const auto &file: files) {
            filesSet.emplace(file);
        }
        files_ = filesSet;
    }

    Commit(const Commit &rhs) = default;

    Commit &operator=(const Commit &rhs) = default;

    Commit(Commit &&rhs) noexcept:
            files_(std::move(rhs.files_)),
            message_(std::move(rhs.message_)) {}

    Commit &operator=(Commit &&rhs) noexcept = default;

public:
    [[nodiscard]]
    auto Files() const -> std::set<File> {
        return files_;
    }

    [[nodiscard]]
    constexpr auto Message() const -> std::string {
        return message_;
    }

public:
    [[nodiscard]]
    auto ToJson() const -> nlohmann::json {
        nlohmann::json j;
        std::vector<nlohmann::json> files;
        for (const auto &file: files_) {
            files.push_back(file.ToJson());
        }
        j["files"] = files;
        j["message"] = message_;
        return j;
    }

    static Commit FromJson(nlohmann::json json) {
        std::set<File> files;
        for (auto &file: json["files"]) {
//            std::cout << file.dump(2) << std::endl;
            files.insert(File::FromJson(file));
        }
        std::string message = json["message"];
        return {files, message};
    }

private:
    std::set<File> files_;
    std::string message_;
};


#endif //MYGIT_COMMIT_H
