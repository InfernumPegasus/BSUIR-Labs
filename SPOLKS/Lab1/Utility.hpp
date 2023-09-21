#ifndef SERVER_UTILITY_HPP
#define SERVER_UTILITY_HPP

#include <ranges>
#include <vector>

inline constexpr auto DEFAULT_IP = "127.0.0.1";
inline constexpr auto DEFAULT_PORT = 8080;

inline constexpr auto EXIT_COMMAND = "exit";
inline constexpr auto ECHO_COMMAND = "echo";
inline constexpr auto TIME_COMMAND = "time";

std::vector<std::string> SplitString(const std::string& str, char delimiter) {
  using namespace std::ranges;
  auto split = str | views::split(delimiter) | views::transform([](auto&& str) {
                 return std::string_view(&*str.begin(), distance(str));
               });
  return {split.begin(), split.end()};
}

#endif  // SERVER_UTILITY_HPP
