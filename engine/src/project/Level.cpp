#include "Level.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool Level::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Level: failed to open \"" << path << "\"\n";
        return false;
    }
    objects.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        if (!(ss >> type)) continue;   // blank line
        if (type[0] == '#') continue;  // comment
        LevelObject obj;
        obj.type = type;
        ss >> obj.position.x >> obj.position.y >> obj.position.z;
        objects.push_back(obj);
    }
    return true;
}

bool Level::save(const std::string& path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Level: failed to write \"" << path << "\"\n";
        return false;
    }
    file << "# RubbleEngine level\n";
    for (const auto& o : objects)
        file << o.type << ' ' << o.position.x << ' '
             << o.position.y << ' ' << o.position.z << '\n';
    return true;
}
