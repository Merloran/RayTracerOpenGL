#pragma once

struct GPUVertex
{
    glm::vec3 position; alignas(16)
    glm::vec3 normal; alignas(16)
    glm::vec2 uv;
    Int32 materialId;
};
