#ifndef MYGIT_REPOSITORY_H
#define MYGIT_REPOSITORY_H

#include <filesystem>
#include "File.h"
#include "Commit.h"

// TODO сделать ignore-файлы
class Repository {
public:
    // TODO сделать занесение файлов
    // TODO сделать валидацию имени папки типа "../../folder/"
    // TODO сделать валидацию имени репозитория
    // TODO сделать проверку на наличие write прав
    Repository(std::string repositoryName, std::string repositoryFolder) :
            repositoryName_(std::move(repositoryName)),
            repositoryFolder_(std::move(repositoryFolder)) {
        if (!std::filesystem::exists(repositoryFolder_)) {
            throw std::invalid_argument("'"s
                                                .append(repositoryFolder_)
                                                .append("' does not exist."));
        }
        if (!std::filesystem::is_directory(repositoryFolder_)) {
            throw std::invalid_argument("'"s
                                                .append(repositoryFolder_)
                                                .append("' is not a folder."));
        }
        commits_.emplace_back(std::vector<File>(), "Initial commit =)");
    }

public:
    auto CollectFiles() -> std::vector<File> {
        std::vector<File> files;

        for (auto &file :
                std::filesystem::recursive_directory_iterator(repositoryFolder_)) {
            if (std::filesystem::is_regular_file(file)) {
                files.emplace_back(file.path());
            }
        }

        return files;
    }

public:
    [[nodiscard]]
    constexpr auto GetName() const -> std::string {
        return repositoryName_;
    }

    [[nodiscard]]
    constexpr auto GetFolder() const -> std::string {
        return repositoryFolder_;
    }

    [[nodiscard]]
    constexpr auto GetCommits() const -> std::vector<Commit> {
        return commits_;
    }

private:
    std::string repositoryName_;
    std::string repositoryFolder_;

    std::vector<Commit> commits_;
};


#endif //MYGIT_REPOSITORY_H
