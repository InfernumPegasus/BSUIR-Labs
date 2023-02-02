#ifndef MYGIT_FILESTATUS_H
#define MYGIT_FILESTATUS_H


enum class FileStatus {
    Unknown = -1,
    Created = 0,
    Modified = 1,
    Deleted = 2,
    Exists = 4
};


#endif //MYGIT_FILESTATUS_H
