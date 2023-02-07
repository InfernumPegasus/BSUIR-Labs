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
               std::string_view repositoryFolder) :
            repositoryName_(repositoryName),
            repositoryFolder_(std::filesystem::absolute(repositoryFolder)),
            ignoreFile_(repositoryFolder_ + "/config/ignore") {
        if (LoadConfigFile()) {
            std::cout << "Config loaded!\n";
        } else if (CreateConfigFile()) {
            std::cout << "Config file created!\n";
        }

        if (!ReadIgnoreFile()) {
            std::cerr << "Error with ignore file!";
            CreateIgnoreFile();
            ReadIgnoreFile();
        }
    }

private:
    Repository(std::string_view repositoryName,
               std::string_view repositoryFolder,
               std::vector<Commit> commits)
            : repositoryName_(repositoryName),
              repositoryFolder_(repositoryFolder),
              commits_(std::move(commits)) {}

private:

    static bool ExistFile(std::string_view filename) {
        return std::filesystem::exists(filename);
    }

    bool CreateIgnoreFile() {
        std::ofstream ofs;
        ofs.open(ignoreFile_, std::ios_base::ate);
        if (!ofs.is_open()) {
            return false;
        }

        for (auto &file: std::filesystem::recursive_directory_iterator(repositoryFolder_)) {
            auto filename = std::filesystem::absolute(file).filename().string();

            if (filename.starts_with(".")) {
                auto absolute = std::filesystem::absolute(file).string() + "\n";
                ofs.write(absolute.c_str(), (long) absolute.length());
            }
        }

        return true;
    }

    bool ReadIgnoreFile() {
        if (!ExistFile(ignoreFile_) ||
            std::filesystem::is_empty(ignoreFile_)) {
            return false;
        }

        std::ifstream ifs(ignoreFile_);
        if (!ifs.is_open()) {
            return false;
        }

        std::string readFilename;
        while (ifs.good()) {
            std::getline(ifs, readFilename);

            if (!ExistFile(readFilename)) {
                continue;
            }

            if (readFilename.starts_with(".") || readFilename.starts_with("..")) {
                ignoredFiles_.insert(readFilename);
            }

            // TODO удалить
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

        return true;
    }

    [[nodiscard]]
    auto CollectFiles() const -> std::set<std::string> {
        std::set<std::string> collectedFiles;

        // Check if a file was created or modified
        for (auto &file: std::filesystem::recursive_directory_iterator(repositoryFolder_)) {
            auto filename = std::filesystem::absolute(file).string();

            // ignore folders and files in ignore file
            if (!std::filesystem::is_directory(filename) &&
                !ignoredFiles_.contains(filename)) {
                collectedFiles.insert(filename);
            }

        }

        return collectedFiles;
    }

    [[nodiscard]] bool CreateConfigFile() const {
        std::string configDirectory = repositoryFolder_ + "/" + VSC_DIRECTORY;
        std::string configFile = configDirectory + "/" + VSC_REPOSITORY_INFO_FILE;

        if ((ExistFile(configDirectory) &&
             !ExistFile(configFile)) ||
            std::filesystem::create_directory(configDirectory)) {
            std::ofstream file(configFile);
            if (file.is_open()) {
                return true;
            }
        }
        return false;
    }

public:
    void RemoveAllCommits() {
        commits_.clear();
    }

    void UpdateConfigFile() const {
        std::string configDirectory = repositoryFolder_ + "/" + VSC_DIRECTORY;
        std::string configFile = configDirectory + "/" + VSC_REPOSITORY_INFO_FILE;

        std::ofstream ofs(configFile);
        if (!ofs.is_open() && !CreateConfigFile()) {
            std::cerr << "Cannot create config folder!";
            return;
        }

        auto repoJson = ToJson().dump(2);
        ofs.write(repoJson.c_str(), static_cast<long>(repoJson.length()));
    }

private:
    // true means successful load, false otherwise
    bool LoadConfigFile() {
        std::string configDirectory = repositoryFolder_ + "/" + VSC_DIRECTORY;
        std::string configFile = configDirectory + "/" + VSC_REPOSITORY_INFO_FILE;

        if (!ExistFile(configFile)) {
            return false;
        }

        if (std::filesystem::is_empty(configFile)) {
            return true;
        }

        std::ifstream ofs(configFile);
        if (ofs.is_open()) {
            nlohmann::json j = nlohmann::json::parse(ofs);

            std::vector<nlohmann::json> commits = j["commits"];
            std::vector<Commit> commitsVector;

            for (auto &commit: commits) {
                commitsVector.emplace_back(Commit::FromJson(commit));
            }

            repositoryName_ = j["repo_name"];
            repositoryFolder_ = j["repo_folder"];
            commits_ = commitsVector;

            return true;
        }

        return false;
    }

public:

    // TODO трекать изменения с последнего коммита
    void DoCommit(std::string message) {
        auto collectedFiles = CollectFiles();

        for (const auto &file: collectedFiles) {
            auto filename = std::filesystem::absolute(file).string();
            auto currentFileLastTimeWrite = std::filesystem::last_write_time(file);

////             File modification
//            if (fileTimestampMap_[filename] != currentFileLastTimeWrite) {
//                fileTimestampMap_[filename] = currentFileLastTimeWrite;
//            }
        }

        std::set<File> files;
        for (const auto &item: collectedFiles) {
            File f(item);
            files.insert(f);
        }
        commits_.emplace_back(files, std::move(message));
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
        std::for_each(commits_.begin(), commits_.end(),
                      [&](const Commit &commit) {
                          commitsJson.push_back(commit.ToJson());
                      });

        j["repo_name"] = repositoryName_;
        j["repo_folder"] = repositoryFolder_;
        j["commits"] = commitsJson;

        return j;
    }

    static auto FromJson(nlohmann::json json) -> Repository {
        std::string repoName = json["repo_name"];
        std::string repoFolder = json["repo_folder"];
        std::vector<nlohmann::json> commitsJson = json["commits"];
        std::vector<Commit> commits;
        std::for_each(commitsJson.begin(), commitsJson.end(),
                      [&](nlohmann::json &commit) {
                          commits.push_back(Commit::FromJson(commit));
                      });

        return {repoName, repoFolder, commits};

    }

private:
    static constexpr std::string VSC_DIRECTORY = "config";
    static constexpr std::string VSC_REPOSITORY_INFO_FILE = "repo_info.json";
    static constexpr std::string VSC_IGNORE_FILE = "ignore";

private:
    std::string repositoryName_;
    std::string repositoryFolder_;

//    std::map<std::string, std::filesystem::file_time_type> fileTimestampMap_;
    std::vector<Commit> commits_;

    std::string ignoreFile_;
    std::set<std::string> ignoredFiles_;
};


#endif //MYGIT_REPOSITORY_H
