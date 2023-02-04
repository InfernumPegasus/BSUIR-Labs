#ifndef MYGIT_REPOSITORY_H
#define MYGIT_REPOSITORY_H

#include "File.h"
#include "Commit.h"
#include <unordered_map>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <regex>

// TODO сделать ignore-файлы
class Repository {
public:
    // TODO сделать валидацию имени папки типа "../../folder/"
    // TODO сделать валидацию имени репозитория
    Repository(std::string_view repositoryName, std::string_view repositoryFolder) :
            repositoryName_(repositoryName),
            repositoryFolder_(std::filesystem::absolute(repositoryFolder)) {
        if (LoadConfig()) {
            std::cout << "Config loaded!\n";
        } else if (CreateConfig()) {
            std::cout << "Config file created!\n";
            commits_.emplace_back(std::vector<std::string>(), "Initial commit =)");
        }
    }

    // TODO добавить условия для сохранения
//    ~Repository() {
//        SaveConfig();
//    }

private:
    void Action(std::string_view file, FileStatus status) {
        switch (status) {
            case FileStatus::Created:
                std::cout << "File created: " << file << '\n';
                break;
            case FileStatus::Modified:
                std::cout << "File modified: " << file << '\n';
                break;
            case FileStatus::Deleted:
                std::cout << "File erased: " << file << '\n';
                break;
            default:
                std::cout << "Error! Unknown file status.\n";
        }
    }

    // TODO сделать избирательность выбора файлов из папок
    void CollectFiles() {
        auto it = fileTimestampMap_.begin();
        while (it != fileTimestampMap_.end()) {
            // if item does not exist
            if (!std::filesystem::exists(it->first)) {
                Action(it->first, FileStatus::Deleted);
                it = fileTimestampMap_.erase(it);
            } else {
                it++;
            }
        }

        // Check if a file was created or modified
        for (auto &file: std::filesystem::recursive_directory_iterator(repositoryFolder_)) {
            // ignore folders
            if (std::filesystem::is_directory(file)) {
                continue;
            }
            auto currentFileLastTimeWrite = std::filesystem::last_write_time(file);
            auto filename = file.path().filename().string();

            // File creation
            if (!fileTimestampMap_.contains(filename)) {
                fileTimestampMap_[filename] = currentFileLastTimeWrite;
                Action(filename, FileStatus::Created);
                // File modification
            } else if (fileTimestampMap_[filename] != currentFileLastTimeWrite) {
                fileTimestampMap_[filename] = currentFileLastTimeWrite;
                Action(filename, FileStatus::Modified);
            }
        }
    }

    bool CreateConfig() const {
        std::string configDirectory = repositoryFolder_ + "/" + VSC_DIRECTORY;
        std::string configFile = configDirectory + "/" + VSC_REPOSITORY_INFO_FILE;

        if ((std::filesystem::exists(configDirectory) &&
             !std::filesystem::exists(configFile)) ||
            std::filesystem::create_directory(configDirectory)) {
            std::ofstream file(configFile);
            if (file.is_open()) {
                return true;
            }
        }
        return false;
    }

    void SaveConfig() const {
        std::string configDirectory = repositoryFolder_ + "/" + VSC_DIRECTORY;
        std::string configFile = configDirectory + "/" + VSC_REPOSITORY_INFO_FILE;

        std::ofstream ofs(configFile);
        if (!ofs.is_open() && !CreateConfig()) {
            std::cerr << "Cannot create config folder!";
            return;
        }

        auto repoJson = ToJson().dump(2);
        ofs.write(repoJson.c_str(), static_cast<long>(repoJson.length()));
    }

    // true means successful load, false otherwise
    bool LoadConfig() {
        std::string configDirectory = repositoryFolder_ + "/" + VSC_DIRECTORY;
        std::string configFile = configDirectory + "/" + VSC_REPOSITORY_INFO_FILE;

        if (!std::filesystem::exists(configFile)) {
            return false;
        }

        if (std::filesystem::is_empty(configFile)) {
            return true;
        }

        std::ifstream ofs(configFile);
        if (ofs.is_open()) {
            nlohmann::json j = nlohmann::json::parse(ofs);
            std::string repoName = j["repo_name"];
            std::string repoFolder = j["repo_folder"];
            for (auto & file : j["tracked_files"]) {
                fileTimestampMap_[file] = std::filesystem::last_write_time(file);
            }

            std::vector<nlohmann::json> commits = j["commits"];
            std::vector<Commit> commitsVector;

            for (auto &commit: commits) {
                commitsVector.emplace_back(commit["file_names"], commit["message"]);
            }

            repositoryName_ = repoName;
            repositoryFolder_ = repoFolder;
            commits_ = commitsVector;

            return true;
        }

        return false;
    }

public:

    [[deprecated]]
    void AddCommit(const Commit &commit) {
        commits_.push_back(commit);
    }

    [[deprecated]]
    void AddCommits(const std::vector<Commit> &commits) {
        for (auto & c : commits) {
            commits_.push_back(c);
        }
    }

    void DoCommit() {}

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
    auto ToJson() const -> nlohmann::json {
        nlohmann::json j;
        std::vector<nlohmann::json> commitsJson;
        std::vector<std::string> filesJson;

        for (auto &commit: commits_) {
            commitsJson.push_back(commit.ToJson());
        }
        for (auto &[file, _]: fileTimestampMap_) {
            filesJson.push_back(file);
        }

        j["repo_name"] = repositoryName_;
        j["repo_folder"] = repositoryFolder_;
        j["tracked_files"] = filesJson;
        j["commits"] = commitsJson;

        return j;
    }

//    [[nodiscard]]
//    auto Files() const -> std::unordered_map<File, FileStatus> {
//        return files_;
//    }

private:
    static constexpr std::string VSC_DIRECTORY = "config";
    static constexpr std::string VSC_REPOSITORY_INFO_FILE = "repo_info.json";
    static constexpr std::string VSC_REPOSITORY_COMMITS_FILE = "commits.json";

private:
    std::string repositoryName_;
    std::string repositoryFolder_;

    std::unordered_map<std::string, std::filesystem::file_time_type> fileTimestampMap_;
    std::vector<Commit> commits_;
};


#endif //MYGIT_REPOSITORY_H
