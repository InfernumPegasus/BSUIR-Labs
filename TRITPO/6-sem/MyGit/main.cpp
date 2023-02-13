#include "entity/repository/Repository.h"

using namespace std::string_literals;

int main() {
    Repository repository("Test-Rep",
                          "/home/vladimir/GitHub/BSUIR-Labs/TRITPO/6-sem/MyGit");
    repository.Init();
    repository.DoCommit("QWERTY");

    return 0;
}
