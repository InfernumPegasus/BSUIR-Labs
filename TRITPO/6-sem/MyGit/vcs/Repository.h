#ifndef MYGIT_REPOSITORY_H
#define MYGIT_REPOSITORY_H

#include "File.h"
#include "Commit.h"
#include <unordered_map>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <regex>
#include <utility>
#include <set>

class Repository {
public:
    // TODO сделать валидацию имени папки типа "../../folder/"
    // TODO сделать валидацию имени репозитория
    Repository(std::string_view repositoryName,
               std::string_view repositoryFolder,
               std::string_view ignoreFile = "/home/vladimir/GitHub/BSUIR-Labs/TRITPO/6-sem/MyGit/config/ignore") :
            repositoryName_(repositoryName),
            repositoryFolder_(std::filesystem::absolute(repositoryFolder)) {
        if (LoadConfig()) {
            std::cout << "Config loaded!\n";
        } else if (CreateConfig()) {
            std::cout << "Config file created!\n";
        }
        if (commits_.empty()) {
            commits_.emplace_back(std::set<std::string>(), "Initial commit =)"s);
        }

        ConfigIgnore(ignoreFile);
    }

private:
    void ConfigIgnore(std::string_view path) {
        std::ifstream ifs(path.data());
        if (!ifs.is_open()) {
            std::cerr << "Cannot open ignore file!";
            return;
        }
        std::string readFilename;
        while (ifs.good()) {
            std::getline(ifs, readFilename);

            if (!std::filesystem::exists(readFilename)) {
                continue;
            }

            if (readFilename.starts_with(".") || readFilename.starts_with("..")) {
                ignoredFiles_.insert(readFilename);
            }

            if (std::filesystem::is_regular_file(readFilename)) {
                ignoredFiles_.insert(readFilename);
            }

            if (std::filesystem::is_directory(readFilename)) {
                for (auto &file:
                        std::filesystem::recursive_directory_iterator(readFilename)) {
                    auto filename = std::filesystem::absolute(file).string();
                    ignoredFiles_.insert(filename);
                }
            }
        }

        for (const auto &item: ignoredFiles_) {
            std::cout << item << "\n";
        }
    }

    auto CollectFiles() -> std::set<std::string> {
        std::set<std::string> collectedFiles;

        auto it = fileTimestampMap_.begin();
        while (it != fileTimestampMap_.end()) {
            // if item does not exist
            if (!std::filesystem::exists(it->first)) {
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
            auto filename = std::filesystem::absolute(file).string();

            // удаление файлов из ignore-файла
            if (ignoredFiles_.contains(filename)) {
                std::cout << "File " << filename << " is in ignore file!\n";
                fileTimestampMap_.erase(filename);
                continue;
            }

            // File creation or modification
            if (!fileTimestampMap_.contains(filename) ||
                fileTimestampMap_[filename] != currentFileLastTimeWrite) {
                fileTimestampMap_[filename] = currentFileLastTimeWrite;
                collectedFiles.insert(filename);
            }
        }

        return collectedFiles;
    }

    [[nodiscard]] bool CreateConfig() const {
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

public:
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

private:
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
            for (auto &file: j["tracked_files"]) {
                fileTimestampMap_[file] = std::filesystem::last_write_time(file);
            }

            std::vector<nlohmann::json> commits = j["commits"];
            std::vector<Commit> commitsVector;

            for (auto &commit: commits) {
                commitsVector.emplace_back(commit["file_names"], commit["message"]);
            }

            repositoryName_ = j["repo_name"];
            repositoryFolder_ = j["repo_folder"];
            commits_ = commitsVector;

            return true;
        }

        return false;
    }

public:
    void DoCommit(std::string message) {
        auto collectedFiles = CollectFiles();

        for (const auto &file: collectedFiles) {
            if (fileTimestampMap_.contains(file)) {
                fileTimestampMap_.erase(file);
            }
        }

        Commit newCommit(collectedFiles, std::move(message));
        commits_.push_back(newCommit);
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

private:
    static constexpr std::string VSC_DIRECTORY = "config";
    static constexpr std::string VSC_REPOSITORY_INFO_FILE = "repo_info.json";
    static constexpr std::string VSC_IGNORE_FILE = "ignore";
//    static constexpr std::string VSC_REPOSITORY_COMMITS_FILE = "commits.json";

private:
    std::string repositoryName_;
    std::string repositoryFolder_;

    std::map<std::string, std::filesystem::file_time_type> fileTimestampMap_;
    std::vector<Commit> commits_;

    std::set<std::string> ignoredFiles_;
};


#endif //MYGIT_REPOSITORY_H
