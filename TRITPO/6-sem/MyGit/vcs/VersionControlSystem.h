#ifndef MYGIT_VERSIONCONTROLSYSTEM_H
#define MYGIT_VERSIONCONTROLSYSTEM_H

#include "Repository.h"

class VersionControlSystem {
public:
    void CreateRepository(std::string repositoryName) {}
    void DeleteRepository(std::string_view repositoryName) {}

private:
    std::vector<Repository> repos_;
};


#endif //MYGIT_VERSIONCONTROLSYSTEM_H
