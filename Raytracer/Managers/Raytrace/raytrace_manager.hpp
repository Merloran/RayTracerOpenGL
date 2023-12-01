#pragma once
#include "../Render/Common/shader.hpp"

struct GpuMaterial
{
	Int32 albedo;
	Int32 normal;
	Int32 roughness;
	Int32 metalness;
	Int32 emission;
};

struct Camera;

class SRaytraceManager
{
public:
	SRaytraceManager(SRaytraceManager&) = delete;
	static SRaytraceManager& get();

	Void startup();

	Void update(Camera& camera);
	Void resize_opengl_texture(UInt32& texture, const glm::ivec2 &size);

	Void shutdown();

private:
	SRaytraceManager() = default;
	~SRaytraceManager() = default;

	Shader triangle, screen;
	std::vector<glm::vec4> positionsWithMaterial;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> uvs;
	std::vector<UInt32> indexes;
	std::vector<UInt64> textures;
	std::vector<GpuMaterial> materials;
	glm::vec3 originPixel, pixelDeltaU, pixelDeltaV;
	glm::ivec2 imageSize;
	UInt32 ssbo[6]; //Positions, Normals, Uvs, Indexes, TextureHandles, Materials 
	UInt32 screenTextures[2];
	Int32 frameCount;
};

