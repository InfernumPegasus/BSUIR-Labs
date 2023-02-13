#include <fstream>
#include <ranges>
#include "Repository.h"

using FileHashMap = Repository::FileHashMap;

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
            ofs.write(absolute.c_str(), static_cast<long>(absolute.length()));
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
        fileHashMap_ = j["map"];

        return true;
    }

    return false;
}

FileHashMap Repository::CollectFiles() const {
    FileHashMap collectedFiles;

    for (auto &file:
            std::filesystem::recursive_directory_iterator(repositoryFolder_)) {
        auto filename = std::filesystem::absolute(file).string();

        if (ignoredFiles_.contains(filename)) {
            continue;
        }
        if (std::filesystem::is_directory(filename)) {
            continue;
        }

        collectedFiles.emplace(filename, File::CalculateHash(filename));
    }

//    for (const auto &item: collectedFiles) {
//        std::cout << item.first << " " << item.second << std::endl;
//    }
    return collectedFiles;
}

std::set<File> Repository::CollectPreviousFiles() const {
    std::set<File> files;
    for (const auto &commit: std::ranges::reverse_view(commits_)) {
//        auto commitFiles = commit.Files();
//        for (const auto &file: commitFiles) {
//            std::cout << file.ToJson() << std::endl;
//        }

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

void Repository::DoCommit(std::string_view message) {
    using std::cout;
    using std::endl;

    std::set<File> toInsert;
    auto collectedFiles = CollectFiles();
    for (const auto &[filename, hash]: collectedFiles) {
        // File does not exist
        if (!std::filesystem::exists(filename)) {
            cout << filename << " dont exist\n";

            // File exists only in Map and doesn't exist in FS
            if (fileHashMap_.contains(filename)) {
                cout << filename << " dont exist everywhere\n";

                toInsert.emplace(filename, File::CalculateHash(filename));
                fileHashMap_.erase(filename);
            }
            // File exists
        } else if (fileHashMap_.contains(filename)) {
            cout << filename << " exists\n";

            // File exists and modified
            if (fileHashMap_.at(filename) != hash) {
                cout << filename << " modified\n";

                auto calcHash = File::CalculateHash(filename);
                toInsert.emplace(filename, calcHash);
                fileHashMap_.at(filename) = calcHash;
            }
        } else {
            cout << filename << " hz chto\n";

            auto calcHash = File::CalculateHash(filename);
            toInsert.emplace(filename, calcHash);
            fileHashMap_.emplace(filename, calcHash);
        }
//        std::cout << filename << "\n" << hash << "\n\n";
    }
    commits_.emplace_back(toInsert, message.data());
    UpdateConfigFile();

//    auto lastCommitFiles = CollectPreviousFiles();
//
//    std::set<File> files;
//    for (const auto &collectedFile: collectedFiles) {
////        files.insert(collectedFile);
//        if (lastCommitFiles.contains(collectedFile)) {
//            if (collectedFile.Hash() != lastCommitFiles.find(collectedFile)->Hash()) {
//                files.insert(collectedFile);
//            }
//        } else {
//            files.insert(collectedFile);
//        }
//    }
//    commits_.emplace_back(files, std::move(message));
//
//    UpdateConfigFile();
}

void Repository::Init() {
    if (LoadConfigFile()) {
        std::cout << "Config loaded!\n";
    } else if (CreateConfigFile()) {
        std::cout << "Config file created!\n";
    }
    ignoredFiles_.emplace(configFile_);

    if (!ReadIgnoreFile()) {
        std::cout << "Creating ignore file!";
        CreateIgnoreFile();
        ReadIgnoreFile();
    }
    UpdateIgnoreFile();
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

nlohmann::json Repository::ToJson() const {
    nlohmann::json j;
    std::vector<nlohmann::json> commitsJson;
    std::for_each(commits_.begin(), commits_.end(),
                  [&](const Commit &commit) {
                      commitsJson.push_back(commit.ToJson());
                  });

    j["repo_name"] = repositoryName_;
    j["repo_folder"] = repositoryFolder_;
    j["commits"] = commitsJson;
    j["map"] = fileHashMap_;

    return j;
}

Repository Repository::FromJson(nlohmann::json json) {
    std::string repoName = json["repo_name"];
    std::string repoFolder = json["repo_folder"];
    std::vector<nlohmann::json> commitsJson = json["commits"];
    FileHashMap fileHashMap = json["map"];
    std::vector<Commit> commits;
    std::for_each(commitsJson.begin(), commitsJson.end(),
                  [&](nlohmann::json &commit) {
                      commits.push_back(Commit::FromJson(commit));
                  });

    return {repoName, repoFolder, commits, fileHashMap};

}
