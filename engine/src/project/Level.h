#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct LevelObject {
    std::string type;
    glm::vec3   position{0.0f};
};

// A level = a list of objects, serialized as plain text (.rlevel).
class Level {
public:
    std::vector<LevelObject> objects;

    bool load(const std::string& path);        // false on failure
    bool save(const std::string& path) const;  // false on failure
};
