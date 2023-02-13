#ifndef MYGIT_FILE_H
#define MYGIT_FILE_H

#include <filesystem>
#include <nlohmann/json.hpp>
#include "FileStatus.h"

class File {
public:
    File(std::string_view filename,
         std::string_view modTime,
         int64_t hash,
         FileStatus status) : name_(filename),
                              modificationTime_(modTime),
                              hash_(hash),
                              status_(status) {}

    File(const File &rhs) = default;

    File &operator=(const File &rhs) = default;

    File(File &&rhs) noexcept = default;

    File &operator=(File &&rhs) noexcept = default;

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

public:
    static int64_t CalculateHash(std::string_view filename);

    static std::string LastWriteTimeString(std::string_view filename);

private:
    static auto LoadContent(std::string_view filename) -> std::vector<char>;

private:
    std::string name_;
    std::string modificationTime_;

    int64_t hash_;
    FileStatus status_;
};


#endif //MYGIT_FILE_H
