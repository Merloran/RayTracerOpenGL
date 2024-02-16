#pragma once
#include "../Render/Common/shader.hpp"
#include "BVH/bvh_builder.hpp"

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

	Void update(Camera& camera, Float32 deltaTime);
	Void generate_rays(Camera& camera);
	Void resize_opengl_texture(UInt32& texture, const glm::ivec2 &size);

	[[nodiscard]]
	Int32 get_frame_count() const;
	[[nodiscard]]
	glm::vec3 get_background_color() const;

	Void reload_shaders();
	Void shutdown();

	Int32 maxBouncesCount;
	glm::ivec2 imageSize;
private:
	SRaytraceManager() = default;
	~SRaytraceManager() = default;
	static constexpr glm::ivec2 WORKGROUP_SIZE{ 16, 16 };

	Shader rayGeneration, rayTrace, screen;
	BVHBuilder bvh;
	std::vector<glm::vec4> positionsWithMaterial;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> uvs;
	std::vector<UInt32> indexes;
	std::vector<UInt32> emissionTriangles;
	std::vector<UInt64> textures;
	std::vector<GpuMaterial> materials;
	glm::vec3 originPixel, pixelDeltaU, pixelDeltaV, backgroundColor;
	Float32 renderTime;
	UInt32 ssbo[8]; //Positions, Normals, Uvs, Indexes, TextureHandles, Materials, BVHNodes, EmissionTriangles
	UInt32 screenTexture, directionTexture;
	Int32 frameCount, trianglesCount;
};

