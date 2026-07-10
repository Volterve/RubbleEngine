#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
    std::string vertexCode   = loadFile(vertPath);
    std::string fragmentCode = loadFile(fragPath);
    const char* vertexSrc    = vertexCode.c_str();
    const char* fragmentSrc  = fragmentCode.c_str();

    unsigned int vs = compileShader(GL_VERTEX_SHADER,   vertexSrc);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    id_ = glCreateProgram();
    glAttachShader(id_, vs);
    glAttachShader(id_, fs);
    glLinkProgram(id_);

    int success;
    char infoLog[512];
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id_, 512, nullptr, infoLog);
        std::cerr << "Shader link error (" << vertPath << "):\n" << infoLog << '\n';
        glDeleteProgram(id_);
        id_ = 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader() {
    if (id_) glDeleteProgram(id_);
}

void Shader::use() const {
    if (id_) glUseProgram(id_);
}

int Shader::getLocation(const std::string& name) const {
    auto it = locationCache_.find(name);
    if (it != locationCache_.end()) return it->second;
    int loc = glGetUniformLocation(id_, name.c_str());
    locationCache_[name] = loc;
    return loc;
}

unsigned int Shader::compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compile error:\n" << infoLog << '\n';
    }
    return shader;
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(getLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(getLocation(name), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(getLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(getLocation(name), 1, glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, const glm::mat3& matrix) const {
    glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMat4(const std::string& name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string Shader::loadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Shader: failed to open \"" << path << "\"\n";
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}
