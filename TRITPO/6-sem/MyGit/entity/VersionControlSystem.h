#ifndef MYGIT_VERSIONCONTROLSYSTEM_H
#define MYGIT_VERSIONCONTROLSYSTEM_H

#include "repository/Repository.h"

class VersionControlSystem {
public:
    void CreateRepository(std::string_view repositoryName) {}
    void DeleteRepository(std::string_view repositoryName) {}

private:
    std::vector<Repository> repos_;
};


#endif //MYGIT_VERSIONCONTROLSYSTEM_H
