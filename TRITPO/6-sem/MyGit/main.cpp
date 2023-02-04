#include <iostream>
#include "vcs/Repository.h"

using namespace std::string_literals;
using namespace std::string_view_literals;

using namespace std;
using namespace std::filesystem;

int main() {
    Repository repository("Test-Rep",
                          "/home/vladimir/GitHub/BSUIR-Labs/TRITPO/6-sem/MyGit");

//    std::vector<Commit> comms {
//            {std::vector<std::string>{"A.txt", "B.txt", "C.txt"}, "1"},
//            {std::vector<std::string>{"A.txt", }, "2"},
//            {std::vector<std::string>{"A.txt", "B.txt", "C.txt"}, "3"},
//            {std::vector<std::string>{"A.txt", "C.txt"}, "<3"},
//    };
//    repository.AddCommits(comms);
    auto commits = repository.Commits();
    for (auto & i : commits) {
        cout << i.ToJson() << endl;
    }
//    repository.CollectFiles();

//    auto json = repository.ToJson();
//    std::cout << json << std::endl;
//
//    Commit commit(std::vector<std::string> {"A.txt", "B.txt", "C.txt"}, "Test commit");
//    std::cout << commit.ToJson() << std::endl;

    return 0;
}
