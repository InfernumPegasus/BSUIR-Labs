#ifndef MYGIT_COMMIT_H
#define MYGIT_COMMIT_H


#include <set>
#include "../file/File.h"

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

    Commit(Commit &&rhs) noexcept = default;
    Commit &operator=(Commit &&rhs) noexcept = default;

public:
    [[nodiscard]] auto Files() const -> std::set<File>;

    [[nodiscard]] constexpr auto Message() const -> std::string;

public:
    [[nodiscard]] auto ToJson() const -> nlohmann::json;

    static Commit FromJson(nlohmann::json json);

private:
    std::set<File> files_;
    std::string message_;
};


#endif //MYGIT_COMMIT_H
