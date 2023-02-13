#ifndef MYGIT_REPOSITORY_H
#define MYGIT_REPOSITORY_H


#include <iostream>
#include "../commit/Commit.h"

class Repository {
public:
    // TODO сделать валидацию имени папки типа "../../folder/"
    // TODO сделать валидацию имени репозитория
    Repository(std::string_view repositoryName,
               std::string_view repositoryFolder) :
            repositoryName_(repositoryName),
            repositoryFolder_(std::filesystem::absolute(repositoryFolder)),
            configFile_(repositoryFolder_ + "/" + VCS_CONFIG_DIRECTORY + "/" + VCS_CONFIG_FILE),
            ignoreFile_(repositoryFolder_ + "/" + VCS_CONFIG_DIRECTORY + "/" + VCS_IGNORE_FILE) {}

private:
    Repository(std::string_view repositoryName,
               std::string_view repositoryFolder,
               std::vector<Commit> commits) :
            repositoryName_(repositoryName),
            repositoryFolder_(repositoryFolder),
            commits_(std::move(commits)) {}

private:
    bool CreateIgnoreFile();

    bool ReadIgnoreFile();

    void UpdateIgnoreFile();

    // TODO Check if a file was created or modified
    [[nodiscard]] std::set<File> CollectFiles() const;

    [[nodiscard]] std::set<File> CollectPreviousFiles() const;

    [[nodiscard]] bool CreateConfigFile() const;

    void UpdateConfigFile() const;

    // true means successful load, false otherwise
    bool LoadConfigFile();

public:
    void Init();

    // TODO трекать изменения с последнего коммита
    void DoCommit(std::string message);

public:
    [[nodiscard]] constexpr std::string Name() const;

    [[nodiscard]] constexpr std::string Folder() const;

    [[nodiscard]] constexpr std::vector<Commit> Commits() const;

    [[nodiscard]] nlohmann::json ToJson() const;

    static Repository FromJson(nlohmann::json json);

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
