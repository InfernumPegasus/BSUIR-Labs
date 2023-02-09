#include "Commit.h"

auto Commit::Files() const -> std::set<File> {
    return files_;
}

constexpr auto Commit::Message() const -> std::string {
    return message_;
}

auto Commit::ToJson() const -> nlohmann::json {
    nlohmann::json j;
    std::vector<nlohmann::json> files;
    for (const auto &file: files_) {
        files.push_back(file.ToJson());
    }
    j["files"] = files;
    j["message"] = message_;
    return j;
}

Commit Commit::FromJson(nlohmann::json json) {
    std::set<File> files;
    for (auto &file: json["files"]) {
        files.insert(File::FromJson(file));
    }
    std::string message = json["message"];
    return {files, message};
}
