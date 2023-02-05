#include <iostream>
#include "vcs/Repository.h"

using namespace std::string_literals;
using namespace std::string_view_literals;

using namespace std;
using namespace std::filesystem;

int main() {
    Repository repository("Test-Rep",
                          "/home/vladimir/GitHub/BSUIR-Labs/TRITPO/6-sem/MyGit");

    int input;
    std::vector<Commit> comms{
            {std::set<std::string>{"A.txt"s, "B.txt"s, "C.txt"s}, "1"s},
            {std::set<std::string>{"A.txt"s,},                    "2"s},
            {std::set<std::string>{"A.txt"s, "C.txt"s},           "<3"s},
    };
    static int num = 0;

//    for (auto &i: repository.Commits()) {
//        cout << i.ToJson() << endl;
//    }
//    repository.DoCommit("COMMIT" + to_string(num++));
//    for (auto &i: repository.Commits()) {
//        cout << i.ToJson() << endl;
//    }

    while (true) {
        cout << "1 - Print commits\n"
                "2 - Do commit\n"
                "3 - Get folder\n"
                "4 - Save config\n"
                "6 - Exit\n\n"
                "Your option:";

        std::cin >> input;
        switch (input) {
            case 1:
                for (auto &i: repository.Commits()) {
                    cout << i.ToJson() << endl;
                }
                break;
            case 2:
                repository.DoCommit("COMMIT" + to_string(num++));
                break;
            case 3:
                cout << repository.Folder() << endl;
                break;
            case 4:
                repository.SaveConfig();
                break;
            case 6:
                return 0;
            default:
                break;
        }
    }

    return 0;
}
