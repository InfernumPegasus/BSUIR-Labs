#include "entity/repository/Repository.h"

int main() {
    Repository repository("Test-Rep",
                          "/home/vladimir/GitHub/BSUIR-Labs/TRITPO/6-sem/MyGit");
    repository.DoCommit("QWERTY");

    return 0;
}

