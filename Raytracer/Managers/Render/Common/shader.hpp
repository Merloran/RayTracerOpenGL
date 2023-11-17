#pragma once

enum class EShaderType : Int8
{
    None,
    Vertex,
    Geometry,
    Fragment,
    Compute
};

class Shader
{
public:
    // Read shaders from disk and create them
    Void create(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
    Void create(const std::string& computePath);
    Void reload();

    Void use();

    // Setters for uniforms
    Void set_bool (const std::string& name, Bool value);
    Void set_int  (const std::string& name, Int32 value);
    Void set_float(const std::string& name, Float32 value);
    Void set_vec2 (const std::string& name, Float32 x, Float32 y);
    Void set_vec2 (const std::string& name, const glm::vec2& vector);
    Void set_vec3 (const std::string& name, Float32 x, Float32 y, Float32 z);
    Void set_vec3 (const std::string& name, const glm::vec3& vector);
    Void set_vec4 (const std::string& name, Float32 x, Float32 y, Float32 z, Float32 w);
    Void set_vec4 (const std::string& name, const glm::vec4& vector);
    Void set_mat4 (const std::string& name, const glm::mat4& value);
    Void set_block(const std::string& name, UInt32 number);

    static Void s_bind_uniform_buffer(UInt32 uniformBufferObject, UInt32 offset, UInt32 size, Float32* data);
    Void shutdown();

private:
    std::string vertexPath, geometryPath, fragmentPath, computePath;
    UInt32 id = 0U;
    Void check_compile_errors(UInt32 shaderId, EShaderType shaderType);

    static inline UInt32 sActiveShaderId = 0U;
};

