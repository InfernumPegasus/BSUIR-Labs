#ifndef MYGIT_COMMIT_H
#define MYGIT_COMMIT_H


#include <set>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../file/File.h"

// TODO добавить хэш-сумму и уникальный ID коммита
class Commit {
public:
    Commit(const std::set<File> &files, std::string message) :
            message_(std::move(message)) {
        for (const auto &file: files) {
            files_.emplace(file.Name(), File::CalculateHash(file.Name()));
//            std::cout << files_.find(file)->ToToml().dump(2) << std::endl;
        }
    }

    Commit(const std::set<std::string> &files, std::string message) :
            message_(std::move(message)) {
//        std::set<File> filesSet;
        for (const auto &file: files) {
//            FileStatus status{FileStatus::Created};
//            if (!std::filesystem::exists(file)) {
//                status = FileStatus::Deleted;
//            }
            files_.emplace(file, File::CalculateHash(file));
        }
//        files_ = filesSet;
    }

    Commit(const Commit &rhs) = default;

    Commit &operator=(const Commit &rhs) = default;

    Commit(Commit &&rhs) noexcept = default;

    Commit &operator=(Commit &&rhs) noexcept = default;

public:
    [[nodiscard]] std::set<File> Files() const;

    [[nodiscard]] constexpr std::string Message() const;

public:
    [[nodiscard]] nlohmann::json ToJson() const;

    static Commit FromJson(nlohmann::json json);

private:
    std::set<File> files_;
    std::string message_;
};


#endif //MYGIT_COMMIT_H