#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

// Loads/compiles/links a GLSL program and caches uniform locations.
class Shader {
public:
    Shader(const std::string& vertPath, const std::string& fragPath);
    ~Shader();

    bool         isValid() const { return id_ != 0; }
    unsigned int handle()  const { return id_; }

    void use() const;
    void setInt  (const std::string& name, int value)              const;
    void setFloat(const std::string& name, float value)            const;
    void setVec3 (const std::string& name, const glm::vec3& value) const;
    void setVec4 (const std::string& name, const glm::vec4& value) const;
    void setMat3 (const std::string& name, const glm::mat3& matrix) const;
    void setMat4 (const std::string& name, const glm::mat4& matrix) const;

private:
    unsigned int id_ = 0;
    mutable std::unordered_map<std::string, int> locationCache_;

    int          getLocation  (const std::string& name) const;
    std::string  loadFile     (const std::string& path);
    unsigned int compileShader(unsigned int type, const char* source);
};
