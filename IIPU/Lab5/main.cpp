#include <iostream>
#include <cstring>
#include <fstream>
#include <dirent.h>

using namespace std;

void print_web_cams(const string &web_cams_path) {
    DIR *dir = opendir(web_cams_path.c_str());
    struct dirent *entry;
    if (dir == nullptr) {
        cout << "Wrong path!" << endl;
        exit(1);
    }

    while ( (entry = readdir(dir)) ) {

        if (entry->d_name[0] != '.') {
            auto uevent_file = web_cams_path + strcat(entry->d_name, "/uevent");
            cout << "Opening '" << uevent_file << "' file:" << endl;

            ifstream ifs(uevent_file);
            if (!ifs.is_open()) {
                cout << "Cannot open file!" << endl;
                exit(1);
            }

            string line;
            while (getline(ifs, line)) {
                cout << line << endl;
            }
            cout << endl;
            ifs.close();
        }

    }

}

int main() {

    string web_cams_path = "/sys/class/video4linux/";

    print_web_cams(web_cams_path);

    return 0;
}
