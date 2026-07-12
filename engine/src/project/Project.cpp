#include "Project.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace fs = std::filesystem;

bool Project::open(const std::string& folderPath) {
    folder = folderPath;
    std::ifstream file(folder + "/project.rproj");
    if (!file.is_open()) {
        std::cerr << "Project: no project.rproj in \"" << folder << "\"\n";
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string key;
        ss >> key;
        if (key == "name") std::getline(ss >> std::ws, name);
        else if (key == "startLevel") ss >> startLevel;
    }
    return true;
}

bool Project::create(const std::string& folderPath, const std::string& projectName) {
    folder = folderPath;
    name = projectName;
    startLevel = "levels/level1.rlevel";

    std::error_code ec;
    fs::create_directories(folder + "/levels", ec);
    if (ec) {
        std::cerr << "Project: cannot create \"" << folder << "\": " << ec.message() << "\n";
        return false;
    }

    std::ofstream manifest(folder + "/project.rproj");
    if (!manifest.is_open()) {
        std::cerr << "Project: cannot write manifest\n";
        return false;
    }
    manifest << "name " << name << "\n";
    manifest << "startLevel " << startLevel << "\n";

    // Write an EMPTY starter level
    Level level;
    return level.save(folder + "/" + startLevel);
}

bool Project::loadStartLevel(Level& out) const {
    return out.load(folder + "/" + startLevel);
}
