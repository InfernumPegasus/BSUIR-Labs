#ifndef MYGIT_FILE_H
#define MYGIT_FILE_H

#include <nlohmann/json.hpp>
#include "FileStatus.h"

class File {
public:
    explicit File(const std::string &name) :
            name_(name),
            hash_(CalculateHash(name)) {}

    File(std::string_view name, FileStatus status) :
            name_(name),
            hash_(CalculateHash(name)),
            status_(status) {}

    File(const File &rhs) = default;

    File &operator=(const File &rhs) = default;

    File(File &&rhs) noexcept = default;

    File &operator=(File &&rhs) noexcept = default;

private:
    File(std::string name, size_t hash, FileStatus status) :
            name_(std::move(name)),
            hash_(hash),
            status_(status) {}

public:
    [[nodiscard]] auto Name() const -> std::string;

    [[nodiscard]] auto Hash() const -> std::size_t;

    [[nodiscard]] constexpr auto Status() const -> FileStatus;

public:
    constexpr bool operator==(const File &rhs) const;

    bool operator<(const File &rhs) const;

public:
    [[nodiscard]] auto ToJson() const -> nlohmann::json;

    static File FromJson(nlohmann::json json);

private:
    static auto LoadContent(std::string_view filename) -> std::vector<char>;

public:
    static auto CalculateHash(std::string_view filename) -> size_t;

private:
    size_t hash_;

    std::string name_;

    FileStatus status_{FileStatus::Unknown};
};


#endif //MYGIT_FILE_H
