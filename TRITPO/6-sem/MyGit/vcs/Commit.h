#ifndef MYGIT_COMMIT_H
#define MYGIT_COMMIT_H

#include "File.h"

// TODO добавить хэш-сумму и уникальный ID коммита
class Commit {
public:
    Commit(const std::vector<File> &files, std::string message) :
            files_(files),
            message_(std::move(message)) {}

public:
    [[nodiscard]]
    constexpr auto GetFiles() const -> std::vector<File> {
        return files_;
    }

    [[nodiscard]]
    constexpr auto GetMessage() const -> std::string {
        return message_;
    }

private:
    std::vector<File> files_;
    std::string message_;
};


#endif //MYGIT_COMMIT_H
