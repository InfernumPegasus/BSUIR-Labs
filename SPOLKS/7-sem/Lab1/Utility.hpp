#ifndef SERVER_UTILITY_HPP
#define SERVER_UTILITY_HPP

#include <ranges>
#include <vector>

inline constexpr auto DEFAULT_IP = "127.0.0.1";
inline constexpr auto DEFAULT_PORT = 8080;

inline constexpr auto EXIT_COMMAND = "exit";
inline constexpr auto ECHO_COMMAND = "echo";
inline constexpr auto TIME_COMMAND = "time";

#define DISABLE_COPY_AND_MOVE(className) \
    className(const className&) = delete; \
    className& operator=(const className&) = delete; \
    className(className&&) = delete; \
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

#endif  // SERVER_UTILITY_HPP
