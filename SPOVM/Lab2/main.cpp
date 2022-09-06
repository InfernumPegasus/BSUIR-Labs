#include <iostream>
#include <dirent.h>
#include <string>
#include <unistd.h>
#include <algorithm>
#include <vector>

#define COLOR_RED "\033[1;31;40m"
#define CLEAR_CLS "\033[1;0m"

struct Flags {
    bool    l = false,  // link
            d = false,  // dir
            f = false,  // file
            s = false;  // sort

public:
    void print_flags() const {
        std::cout << "Search flags:" << std::endl << std::boolalpha <<
                  "l_flag : " << l << std::endl <<
                  "d_flag : " << d << std::endl <<
                  "f_flag : " << f << std::endl <<
                  "s_flag : " << s << std::endl;
    }
} flags;

std::string get_initial_directory(int argc, char **argv) {
    if (argc == 1 || argv[1][0] == '-')
        return "./";

    return argv[1];
}

void print_vector(const std::vector<std::string> &vec) {
    for (const auto& item : vec)
        std::cout << item << std::endl;
}

void get_flags(int argc, char **argv) {
    bool has_default_flags = false;

    int c;
    while ( (c = getopt(argc, argv, "ldfs")) != -1) {
        if (c == 'l' || c == 'd' || c == 'f')
            has_default_flags = true;

        switch (c) {
            case 'l':
                flags.l = true;
                break;
            case 'd':
                flags.d = true;
                break;
            case 'f':
                flags.f = true;
                break;
            case 's':
                flags.s = true;
                break;

            default:
                throw std::runtime_error("Wrong options!");
        }
    }
    if (!has_default_flags)
        flags.d = flags.f = flags.l = true;
}

void print(std::string path, std::vector<std::string> &files) {
    DIR *dir;
    struct dirent *entry;

    if (path.back() == '/')
        path.pop_back();

    dir = opendir(path.c_str());

    while ((entry = readdir(dir))) {
        const std::string dirName = entry->d_name;
        std::string temp;

        if (entry->d_type == DT_DIR && (dirName == "." || dirName == "..")) {
            continue;
        }

        bool show_dir_only = false;
        if (flags.d && !flags.l && !flags.f)
            show_dir_only = true;

        if (!show_dir_only && flags.l && entry->d_type == DT_LNK) {
            temp = path + '/' + entry->d_name;
            files.push_back(temp);
            print(temp, files);
        }

        if (!show_dir_only && flags.f && entry->d_type == DT_REG) {
            temp = path + '/' + entry->d_name;
            files.push_back(temp);
        }

        if (flags.d && entry->d_type == DT_DIR) {
            temp = path + '/' + entry->d_name;
//            std::cout << COLOR_RED << temp << CLEAR_CLS << std::endl;
            files.push_back(temp);
            print(temp, files);
        }
    }

    if (flags.s)
        std::sort(files.begin(), files.end());

    print_vector(files);
    files.clear();
    closedir(dir);
}


int main(int argc, char* argv[]) {
    auto path = get_initial_directory(argc, argv);
    std::vector<std::string> files;

    get_flags(argc, argv);
    print(path, files);

    flags.print_flags();

    return 0;
}
