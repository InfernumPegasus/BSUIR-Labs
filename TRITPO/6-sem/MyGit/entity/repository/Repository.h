#ifndef MYGIT_REPOSITORY_H
#define MYGIT_REPOSITORY_H

#include "../file/File.h"
#include "../Commit.h"
#include <iostream>


class Repository {
public:
    // TODO сделать валидацию имени папки типа "../../folder/"
    // TODO сделать валидацию имени репозитория
    Repository(std::string_view repositoryName,
               std::string_view repositoryFolder) :
            repositoryName_(repositoryName),
            repositoryFolder_(std::filesystem::absolute(repositoryFolder)),
            configFile_(repositoryFolder_ + "/" + VCS_CONFIG_DIRECTORY + "/" + VCS_CONFIG_FILE),
            ignoreFile_(repositoryFolder_ + "/" + VCS_CONFIG_DIRECTORY + "/" + VCS_IGNORE_FILE) {
        if (LoadConfigFile()) {
            std::cout << "Config loaded!\n";
        } else if (CreateConfigFile()) {
            std::cout << "Config file created!\n";
        }

        if (!ReadIgnoreFile()) {
            std::cout << "Creating ignore file!";
            CreateIgnoreFile();
            ReadIgnoreFile();
        }
        UpdateIgnoreFile();
    }

private:
    Repository(std::string_view repositoryName,
               std::string_view repositoryFolder,
               std::vector<Commit> commits) :
            repositoryName_(repositoryName),
            repositoryFolder_(repositoryFolder),
            commits_(std::move(commits)) {}

private:
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
        if (!std::filesystem::exists(ignoreFile_) ||
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
            if (!std::filesystem::exists(readFilename)) {
                continue;
            }

            ignoredFiles_.emplace(readFilename);
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

    void UpdateIgnoreFile() {
        std::ofstream ofs;
        ofs.open(ignoreFile_);
        if (ofs.is_open()) {
            for (const auto &item: ignoredFiles_) {
                std::string file = item + "\n";
                ofs.write(file.c_str(), (long) file.length());
            }
        }
    }

    // TODO Check if a file was created or modified
    [[nodiscard]] auto CollectFiles() const -> std::set<File> {
        std::set<File> collectedFiles;

        for (auto &file:
                std::filesystem::recursive_directory_iterator(repositoryFolder_)) {
            auto filename = std::filesystem::absolute(file).string();

            if (!ignoredFiles_.contains(filename) &&
                !std::filesystem::is_directory(filename)) {
                collectedFiles.emplace(filename);
            }
        }

        return collectedFiles;
    }

    [[nodiscard]] bool CreateConfigFile() const {
        std::string configDirectory = repositoryFolder_ + "/" + VCS_CONFIG_DIRECTORY;
        std::ofstream file;
        if ((std::filesystem::exists(configDirectory) &&
             !std::filesystem::exists(configFile_)) ||
            std::filesystem::create_directory(configDirectory)) {}
        file.open(configFile_);
        return file.is_open();
    }

    void UpdateConfigFile() const {
        std::ofstream ofs(configFile_);
        if (!ofs.is_open() && !CreateConfigFile()) {
            std::cout << "Cannot create config folder!";
            return;
        }

        auto repoJson = ToJson().dump(2);
        ofs.write(repoJson.c_str(), static_cast<long>(repoJson.length()));
    }

    // true means successful load, false otherwise
    bool LoadConfigFile() {
        if (!std::filesystem::exists(configFile_)) {
            return false;
        }

        if (std::filesystem::is_empty(configFile_)) {
            return true;
        }

        std::ifstream ofs(configFile_);
        if (ofs.is_open()) {
            nlohmann::json j = nlohmann::json::parse(ofs);
            std::vector<Commit> commitsVector;

            for (auto &commit: j["commits"]) {
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

    [[nodiscard]] auto CollectPreviousFiles() const -> std::set<File> {
        std::set<File> files;
        for (const auto &commit: std::ranges::reverse_view(commits_)) {
            files.merge(commit.Files());
        }
        return files;
    }

    // TODO трекать изменения с последнего коммита
    void DoCommit(std::string message) {
        // if no commits
        if (commits_.empty()) {
            commits_.emplace_back(std::set<File>(), "Initial commit =)");
        }

        auto collectedFiles = CollectFiles();
        auto lastCommitFiles = CollectPreviousFiles();

        std::set<File> files;
        for (const auto &collectedFile: collectedFiles) {
            if (lastCommitFiles.contains(collectedFile)) {
                if (collectedFile.Hash() != lastCommitFiles.find(collectedFile)->Hash()) {
                    files.insert(collectedFile);
                }
            } else {
                files.insert(collectedFile);
            }
        }
        commits_.emplace_back(files, std::move(message));

        UpdateConfigFile();
    }

public:
    [[nodiscard]] constexpr auto Name() const -> std::string {
        return repositoryName_;
    }

    [[nodiscard]] constexpr auto Folder() const -> std::string {
        return repositoryFolder_;
    }

    [[nodiscard]] constexpr auto Commits() const -> std::vector<Commit> {
        return commits_;
    }

    [[nodiscard]] auto ToJson() const -> nlohmann::json {
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
    static constexpr std::string VCS_CONFIG_DIRECTORY = "config";
    static constexpr std::string VCS_CONFIG_FILE = "repo_info.json";
    static constexpr std::string VCS_IGNORE_FILE = "ignore";

private:
    std::string repositoryName_;
    std::string repositoryFolder_;

    std::string configFile_;
    std::string ignoreFile_;

    std::vector<Commit> commits_;
    std::set<std::string> ignoredFiles_;
};


#endif //MYGIT_REPOSITORY_H
