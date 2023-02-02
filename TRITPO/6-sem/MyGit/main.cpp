#include <iostream>
#include "vcs/Repository.h"

using namespace std::string_literals;
using namespace std::string_view_literals;

using namespace std;
using namespace std::filesystem;

int main() {
    Repository repository("Test-Rep",
                          "/home/vladimir/GitHub/BSUIR-Labs/TRITPO/6-sem/MyGit");
    repository.CollectFiles();

    auto json = repository.ToJson();
    std::cout << json << std::endl;

    Commit commit(std::vector<std::string> {"A.txt", "B.txt", "C.txt"}, "Test commit");
    std::cout << commit.ToJson() << std::endl;

//    unordered_map<std::string, file_time_type> file_timestamp_map;
//    for (;;) {
//        for (auto &file: directory_iterator(kWatchedPath)) {
//            bool is_changed = false;
//            // check only files
//            if (!is_regular_file(file)) {
//                continue;
//            }
//
//            const auto filename = file.path().filename().string();
//            auto ts = last_write_time(file);
//
//            if (file_timestamp_map.contains(filename)) {
//                auto &last_ts = file_timestamp_map[filename];
//                if (ts != last_ts) {
//                    is_changed = true;
//                }
//                last_ts = ts;
//            } else {
//                file_timestamp_map[filename] = ts;
//                is_changed = true;
//            }
//
//            if (!is_changed) {
//                continue;
//            }
//
//            std::string alias = filename;
//            std::regex_replace(
//                    alias.begin(), filename.begin(), filename.end(), std::regex("\\."),
//                    "_");  // we use filename as an entry name. It can't contain dots.
//            std::cout << "`" << filename << "` is changed. Storing as `" << alias
//                      << "` ";
//
//            ifstream changed_file(file.path());
//            if (!changed_file) {
//                std::cerr << "Failed open file";
//                return -1;
//            }

    return 0;
}
