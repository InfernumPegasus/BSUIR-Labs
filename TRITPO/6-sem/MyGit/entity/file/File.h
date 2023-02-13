#ifndef MYGIT_FILE_H
#define MYGIT_FILE_H

#include <filesystem>
#include <nlohmann/json.hpp>
#include <utility>
#include "FileStatus.h"

class File {
public:
//    File(const std::string &name, FileStatus status) :
//            status_(status),
//            name_(name) {}

//    File(std::string_view name, FileStatus status) :
//            status_(status),
//            name_(name) {}

    File(const File &rhs) :
            File(rhs.name_, rhs.hash_) {}

    File &operator=(const File &rhs) = default;

    File(File &&rhs) noexcept = default;

    File &operator=(File &&rhs) noexcept = default;

    File(std::string_view name, int64_t hash, std::string modificationTime, FileStatus status) :
            name_(name),
            hash_(hash),
            modificationTime_(std::move(modificationTime)),
            status_(status) {}

    File(std::string_view name, int64_t hash) : name_(name) {
        if (!std::filesystem::exists(name)) {
            status_ = FileStatus::Deleted;
            hash_ = 0;
        } else if (auto calculatedHash = CalculateHash(name);
                calculatedHash != hash) {
            status_ = FileStatus::Modified;
            hash_ = calculatedHash;
        } else {
            status_ = FileStatus::Created;
            modificationTime_ = LastWriteTimeString(name);
            hash_ = hash;
        }
    }

public:
    [[nodiscard]] std::string Name() const;

    [[nodiscard]] int64_t Hash() const;

    [[nodiscard]] FileStatus Status() const;

    [[nodiscard]] std::string ModificationTime() const;

public:
    constexpr bool operator==(const File &rhs) const;

    bool operator<(const File &rhs) const;

public:
    [[nodiscard]] nlohmann::json ToJson() const;

    static File FromJson(nlohmann::json json);

private:
    static auto LoadContent(std::string_view filename) -> std::vector<char>;

public:
    static int64_t CalculateHash(std::string_view filename);

    static std::string LastWriteTimeString(std::string_view filename);

private:
    int64_t hash_;
    FileStatus status_;

    std::string name_;
    std::string modificationTime_;
};


#endif //MYGIT_FILE_H
