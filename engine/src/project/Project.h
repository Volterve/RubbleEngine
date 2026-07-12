#pragma once
#include "Level.h"
#include <string>

// A project = a folder with a project.rproj manifest + one or more levels.
class Project {
public:
    std::string folder;      // path to the project directory
    std::string name;
    std::string startLevel;  // path relative to folder

    bool open(const std::string& folderPath);                          // reads project.rproj
    bool create(const std::string& folderPath, const std::string& projectName);
    bool loadStartLevel(Level& out) const;                             // loads the start level
};
