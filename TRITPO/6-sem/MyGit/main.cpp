#include <iostream>
#include "vcs/Repository.h"

using namespace std::string_literals;
using namespace std::string_view_literals;

using namespace std;
using namespace std::filesystem;

int main() {
    Repository repository("Test-Rep",
                          "/home/vladimir/GitHub/BSUIR-Labs/TRITPO/6-sem/MyGit");

//    int input;
//    static int num = 0;
//
//    while (true) {
//        cout << "1 - Print commits\n"
//                "2 - Do commit\n"
//                "3 - Get folder\n"
//                "4 - Save config\n"
//                "6 - Exit\n\n"
//                "Your option:";
//
//        std::cin >> input;
//        switch (input) {
//            case 1:
//                for (auto &i: repository.Commits()) {
//                    cout << i.ToJson().dump(2) << endl;
//                }
//                break;
//            case 2:
//                repository.DoCommit("COMMIT" + to_string(num++));
//                break;
//            case 3:
//                cout << repository.Folder() << endl;
//                break;
//            case 4:
//                repository.UpdateConfigFile();
//                break;
//            case 6:
//                return 0;
//            default:
//                break;
//        }
//    }

//    std::vector<Commit> comms{
//            {std::set<std::string>{"A.txt"s, "B.txt"s, "C.txt"s}, "1"s},
//            {std::set<std::string>{"A.txt"s,},                    "2"s},
//            {std::set<std::string>{"A.txt"s, "C.txt"s},           "<3"s},
//    };


//    repository.RemoveAllCommits();
//    repository.DoCommit("QWERTY");
//    cout << repository.ToJson().dump(2) << endl;
//    for (const auto &item: repository.Commits()) {
//        cout << item.ToJson().dump(2);
//    }
//    repository.DoCommit("SUKA");
    cout << repository.ToJson().dump(2) << endl;
    repository.UpdateConfigFile();


//    std::vector<File> files;
//    File f1("/home/vladimir/GitHub/BSUIR-Labs/TRITPO/6-sem/MyGit/CMakeLists.txt");
//    File f2("/home/vladimir/GitHub/BSUIR-Labs/TRITPO/6-sem/MyGit/vcs/Branch.h");
//    File f3("/home/vladimir/GitHub/BSUIR-Labs/TRITPO/6-sem/MyGit/vcs/FileStatus.h");
//    files.push_back(f1);
//    files.push_back(f2);
//    files.push_back(f3);
//
//    for (const auto &item: files) {
//        std::cout << item.ToJson().dump(2) << endl;
//    }
//
//    Commit commit({f1, f2, f3}, "HAHA"s);
//    std::cout << Commit::FromJson(commit.ToJson()).ToJson().dump(2);

//    auto j1 = repository.ToJson();
//    cout << j1.dump(2) << endl;

//    auto j2 = Repository::FromJson(j1).ToJson();
//    cout << j2.dump(2) << endl;


//    File file("/home/vladimir/GitHub/BSUIR-Labs/TRITPO/6-sem/MyGit/main.cpp");
//    std::cout << file.ToJson().dump(2) << endl;

    return 0;
}
