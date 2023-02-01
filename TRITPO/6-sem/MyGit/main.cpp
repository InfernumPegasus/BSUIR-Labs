#include <iostream>
#include "vcs/Repository.h"

using namespace std::string_literals;
using namespace std::string_view_literals;

// TODO реализовать ветки, возможность их мержить, создавать, удалять и т.д.
class Branch {};


int main() {
    Repository repository("Test-Rep", "./");
    std::cout << repository.GetFolder() << ": " << repository.GetName() << std::endl;
    for (auto & f : repository.CollectFiles()) {
        std::cout << f.Name() << std::endl;
    }

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

//            auto write_err = bucket->Write(
//                    alias, std::chrono::file_clock::to_sys(ts),
//                    [file_size, &changed_file](ReductBucket::WritableRecord* rec) {
//                        rec->Write(file_size, [&](size_t offest, size_t size) {
//                            std::string buffer;
//                            buffer.resize(size);
//                            changed_file.read(buffer.data(), size);
//                            std::cout << "." << std::flush;
//                            return std::pair{offest + size <= file_size, buffer};
//                        });
//                    });

//            if (write_err) {
//                std::cout << " Err:" << write_err << std::endl;
//            } else {
//                std::cout << " OK (" << file_size / 1024 << " kB)" << std::endl;
//        }
//    }

    return 0;
}
