#include <fstream>
#include "Repository.h"

bool Repository::CreateIgnoreFile() {
    std::ofstream ofs;
    ofs.open(ignoreFile_, std::ios_base::ate);
    if (!ofs.is_open()) {
        return false;
    }

    for (auto &file:
            std::filesystem::recursive_directory_iterator(repositoryFolder_)) {
        auto filename = std::filesystem::absolute(file).filename().string();
        if (filename.starts_with(".")) {
            auto absolute = std::filesystem::absolute(file).string() + "\n";
            ofs.write(absolute.c_str(), (long) absolute.length());
        }
    }

    return true;
}

bool Repository::ReadIgnoreFile() {
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

void Repository::UpdateIgnoreFile() {
    std::ofstream ofs;
    ofs.open(ignoreFile_);
    if (ofs.is_open()) {
        for (const auto &item: ignoredFiles_) {
            std::string file = item + "\n";
            ofs.write(file.c_str(), (long) file.length());
        }
    }
}

auto Repository::CollectFiles() const -> std::set<File> {
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

auto Repository::CollectPreviousFiles() const -> std::set<File> {
    std::set<File> files;
    for (const auto &commit:
            std::ranges::reverse_view(commits_)) {
//        auto commitFiles = commit.Files();
//        for (auto &file: commitFiles) {
//            FileStatus status = FileStatus::Unknown;
//            if (!std::filesystem::exists(file.Name())) {
//                status = FileStatus::Deleted;
//            } else if (file.Hash() != File::CalculateHash(file.Name())) {
//                status = FileStatus::Modified;
//            } else {
//                status = FileStatus::Created;
//            }
//            files.emplace(file.Name(), status);
//        }

        files.merge(commit.Files());
    }
    return files;
}

bool Repository::CreateConfigFile() const {
    std::string configDirectory = repositoryFolder_ + "/" + VCS_CONFIG_DIRECTORY;
    std::ofstream file;
    if ((std::filesystem::exists(configDirectory) &&
         !std::filesystem::exists(configFile_)) ||
        std::filesystem::create_directory(configDirectory)) {}
    file.open(configFile_);
    return file.is_open();
}

void Repository::UpdateConfigFile() const {
    std::ofstream ofs(configFile_);
    if (!ofs.is_open() && !CreateConfigFile()) {
        std::cout << "Cannot create config folder!";
        return;
    }

    auto repoJson = ToJson().dump(2);
    ofs.write(repoJson.c_str(), static_cast<long>(repoJson.length()));
}

bool Repository::LoadConfigFile() {
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

void Repository::DoCommit(std::string message) {
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

constexpr auto Repository::Name() const -> std::string {
    return repositoryName_;
}

constexpr auto Repository::Folder() const -> std::string {
    return repositoryFolder_;
}

constexpr auto Repository::Commits() const -> std::vector<Commit> {
    return commits_;
}

auto Repository::ToJson() const -> nlohmann::json {
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

auto Repository::FromJson(nlohmann::json json) -> Repository {
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
