#ifndef MYGIT_REPOSITORY_H
#define MYGIT_REPOSITORY_H

#include <filesystem>
#include <unordered_map>
#include "File.h"
#include "Commit.h"
#include "FileDifference.h"
#include <nlohmann/json.hpp>

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
        CreateConfigFolder();
        commits_.emplace_back(std::vector<std::string>(), "Initial commit =)");
    }

public:
    // TODO сделать избирательность выбора файлов из папок
    void CollectFiles() {
        for (auto &file:
                std::filesystem::recursive_directory_iterator(repositoryFolder_)) {
            if (std::filesystem::is_regular_file(file)) {
                trackedFiles_.emplace_back(file.path());
            }
        }
    }

    // TODO сделать заполнение конфигурационного файла (файлов)
    // TODO сделать восстановление состояния системы из конфигурационных файлов
    void CreateConfigFolder() {
        std::string configDirectory = repositoryFolder_ + "/" + VSC_DIRECTORY;
        std::string configFile = configDirectory + "/" + VSC_REPOSITORY_INFO_FILE;

        if ((std::filesystem::exists(configDirectory) &&
             !std::filesystem::exists(configFile)) ||
            std::filesystem::create_directory(configDirectory)) {
            std::ofstream file(configFile);
        }
    }

    void DoCommit() {
        const auto lastCommit = commits_.back();
        auto lastCommitFiles = lastCommit.FileNames();
        std::vector<File> filesToCommit;

        for (auto &filename: lastCommitFiles) {
            File file(filename);
            auto status = file.Status();

            // TODO реализовать
            switch (status) {
                case FileStatus::Unknown:
                    break;
                case FileStatus::Exists:
                    break;
                // just add file to new commit
                case FileStatus::Created:
                    filesToCommit.push_back(file);
                    break;
                // determine what have changed
                case FileStatus::Modified:
//                    auto difference = FileDifference();
                    filesToCommit.push_back(file);
                    break;
                case FileStatus::Deleted:
                    break;
            }
            trackedFiles_.erase(
                    std::find(trackedFiles_.begin(),
                              trackedFiles_.end(),
                              file));
        }
    }

    auto ToJson() -> nlohmann::json {
        nlohmann::json j;

        std::vector<nlohmann::json> commitsJson;
        for (auto &commit: commits_) {
            commitsJson.push_back(commit.ToJson());
        }
        std::vector<nlohmann::json> filesJson;
        for (auto &file: trackedFiles_) {
            filesJson.push_back(file.ToJson());
        }

        j["repo_name"] = repositoryName_;
        j["repo_folder"] = repositoryFolder_;
        j["tracked_files"] = filesJson;
        j["commits"] = commitsJson;

        return j;
    }

public:
    [[nodiscard]]
    constexpr auto Name() const -> std::string {
        return repositoryName_;
    }

    [[nodiscard]]
    constexpr auto Folder() const -> std::string {
        return repositoryFolder_;
    }

    [[nodiscard]]
    constexpr auto Commits() const -> std::vector<Commit> {
        return commits_;
    }

    [[nodiscard]]
    constexpr auto Files() const -> std::vector<File> {
        return trackedFiles_;
    }

private:
    static constexpr std::string VSC_DIRECTORY = "config";
    static constexpr std::string VSC_REPOSITORY_INFO_FILE = "gitfile";

private:
    std::string repositoryName_;
    std::string repositoryFolder_;

    std::vector<File> trackedFiles_;
    std::vector<Commit> commits_;
};


#endif //MYGIT_REPOSITORY_H
