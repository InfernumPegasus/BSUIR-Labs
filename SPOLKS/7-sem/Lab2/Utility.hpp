#ifndef SERVER_UTILITY_HPP
#define SERVER_UTILITY_HPP

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

inline constexpr auto DEFAULT_IP = "127.0.0.1";
inline constexpr auto DEFAULT_PORT = 8080;

// clang-format off
inline constexpr auto EXIT_COMMAND                = "exit";
inline constexpr auto EXIT_ALL_COMMAND            = "!exit";
inline constexpr auto ECHO_COMMAND                = "echo";
inline constexpr auto TIME_COMMAND                = "time";
inline constexpr auto DOWNLOAD_COMMAND            = "download";
inline constexpr auto UPLOAD_COMMAND              = "upload";

inline constexpr auto STRING_NO_PREVIOUS_SESSIONS = "NO_PREVIOUS_SESSIONS";
inline constexpr auto STRING_END_INDEX            = "END_INDEX";
// clang-format on

#define DISABLE_COPY_AND_MOVE(className)           \
  className(const className&) = delete;            \
  className& operator=(const className&) = delete; \
  className(className&&) = delete;                 \
  className& operator=(className&&) = delete;

std::vector<std::string> SplitString(const std::string& str, char delimiter) {
  std::vector<std::string> split;
  std::string current;

  for (char c : str) {
    if (c == delimiter) {
      split.push_back(current);
      current.clear();
    } else {
      current += c;
    }
  }

  split.push_back(current);
  return split;
}

std::pair<std::vector<std::string>, int> SplitFile(const std::string& filename, long N) {
  std::vector<std::string> result;

  std::ifstream file(filename, std::ios::binary);

  if (!file) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return {result, -1};
  }

  file.seekg(0, std::ios::end);
  std::streampos fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  const auto numParts = fileSize / N;
  const auto remainingBytes = fileSize % N;

  char* buffer = new char[N];

  for (int i = 0; i < numParts; i++) {
    file.read(buffer, N);
    result.emplace_back(buffer, N);
  }

  if (remainingBytes > 0) {
    file.read(buffer, remainingBytes);
    result.emplace_back(buffer, remainingBytes);
  }

  delete[] buffer;
  file.close();

  return {result, fileSize};
}

std::string GetParameterValueStr(const std::string& str,
                                 const std::string& delimiter = "=") {
  return str.substr(str.find(delimiter) + delimiter.length(), str.length());
}

struct SessionFileInfo {
  std::string file_;
  size_t currentIndex_;
  size_t maxIndex_;
};

#endif  // SERVER_UTILITY_HPP
