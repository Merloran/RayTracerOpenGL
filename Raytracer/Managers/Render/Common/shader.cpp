#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <glad/glad.h>

Void Shader::create(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
    this->vertexPath = vertexPath;
    this->fragmentPath = fragmentPath;
    std::string vShaderCode, fShaderCode, gShaderCode;
    std::ifstream vShaderFile, fShaderFile, gShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // Replace exception with something prettier
    try
    {
        std::stringstream vShaderStream, fShaderStream, gShaderStream;
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        vShaderFile.close();
        fShaderFile.close();
        
        vShaderCode = vShaderStream.str();
        fShaderCode = fShaderStream.str();

        if (!geometryPath.empty())
        {
            this->geometryPath = geometryPath;
            gShaderFile.open(geometryPath);
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            gShaderCode = gShaderStream.str();
        }

    }
    catch (std::ifstream::failure& e)
    {
        SPDLOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: {}", e.what());
        return;
    }
    const Char *vertexShaderCode, *fragmentShaderCode, *geometryShaderCode;

    vertexShaderCode = vShaderCode.c_str();
    fragmentShaderCode = fShaderCode.c_str();
    if (!geometryPath.empty())
    {
        geometryShaderCode = gShaderCode.c_str();
    }
    UInt32 vertex, fragment, geometry;

    // Vertex Shader  
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderCode, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, EShaderType::Vertex);

    // Fragment Shader  
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, EShaderType::Fragment);

    if (!geometryPath.empty())
    {
        // Geometry Shader  
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometryShaderCode, NULL);
        glCompileShader(geometry);
        check_compile_errors(geometry, EShaderType::Geometry);
    }

    // Program Object  
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    if (!geometryPath.empty())
    {
        glAttachShader(id, geometry);
    }
    glLinkProgram(id);
    check_compile_errors(id, EShaderType::None);

    // Deleting shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (!geometryPath.empty())
    {
        glDeleteShader(geometry);
    }
}

Void Shader::create(const std::string& computePath)
{
    this->computePath = computePath;
    std::string computeShaderCode;
    std::ifstream cShaderFile;
    cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        cShaderFile.open(computePath);

        std::stringstream cShaderStream;
        cShaderStream << cShaderFile.rdbuf();
        cShaderFile.close();
        computeShaderCode = cShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        SPDLOG_ERROR("ERROR::COMPUTE::SHADER::FILE_NOT_SUCCESFULLY_READ: {}", e.what());
    }
    const Char* cShaderCode = computeShaderCode.c_str();
    UInt32 compute;
    
    compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &cShaderCode, NULL);
    glCompileShader(compute);
    check_compile_errors(compute, EShaderType::Compute);


    id = glCreateProgram();
    glAttachShader(id, compute);
    glLinkProgram(id);
    check_compile_errors(id, EShaderType::None);

    glDeleteShader(compute);
}

Void Shader::reload()
{
    shutdown();
    if (!geometryPath.empty())
    {
        create(vertexPath, fragmentPath);
    }
    else if (computePath.empty())
    {
        create(vertexPath, fragmentPath, geometryPath);
    } else {
        create(computePath);
    }
}

Void Shader::use()
{
    if (sActiveShaderId != id)
    {
        glUseProgram(id);
        sActiveShaderId = id;
    }
}

Void Shader::set_bool(const std::string& name, Bool value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (Int32)value);
}

Void Shader::set_int(const std::string& name, Int32 value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

Void Shader::set_float(const std::string& name, Float32 value)
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

Void Shader::set_vec2(const std::string& name, Float32 x, Float32 y)
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

Void Shader::set_vec2(const std::string& name, const glm::vec2& vector)
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y);
}

Void Shader::set_vec3(const std::string& name, Float32 x, Float32 y, Float32 z)
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

Void Shader::set_vec3(const std::string& name, const glm::vec3& vector)
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y, vector.z);
}

Void Shader::set_vec4(const std::string& name, Float32 x, Float32 y, Float32 z, Float32 w)
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

Void Shader::set_vec4(const std::string& name, const glm::vec4& vector)
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y, vector.z, vector.w);
}

Void Shader::set_mat4(const std::string& name, const glm::mat4& value)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

Void Shader::set_block(const std::string& name, UInt32 number)
{
    glUniformBlockBinding(id, glGetUniformBlockIndex(id, name.c_str()), number);
}

Void Shader::s_bind_uniform_buffer(UInt32 uniformBufferObject, UInt32 offset, UInt32 size, Float32* data)
{
    glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Void Shader::check_compile_errors(UInt32 shaderId, EShaderType shaderType)
{
    Int32 success;
    Char infoLog[1024];
    std::string name;
    switch (shaderType)
    {
        case EShaderType::Vertex:
            name = "Vertex";
        break;
        case EShaderType::Geometry:
            name = "Geometry";
        break;
        case EShaderType::Fragment:
            name = "Fragment";
        break;
        case EShaderType::Compute:
            name = "Compute";
        break;
        case EShaderType::None:
            name = "Program";
        break;
        default:
            SPDLOG_ERROR("Unhandled shader type.");
        break;
    }

    if (shaderType != EShaderType::None)
    {
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
            SPDLOG_ERROR("ERROR::SHADER_COMPILATION_ERROR::{}\n{}", name, infoLog);
        }
    }
    else
    {
        glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderId, 1024, NULL, infoLog);
            SPDLOG_ERROR("ERROR::PROGRAM_LINKING_ERROR::{}\n{}", name, infoLog);
        }
    }
}

Void Shader::shutdown()
{
    if (id != 0)
    {
        glDeleteProgram(id);
        id = 0;
    }
}