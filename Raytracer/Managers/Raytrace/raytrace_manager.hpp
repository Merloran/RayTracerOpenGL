#pragma once
#include "../Resource/Common/texture.hpp"
#include "../Render/Common/shader.hpp"
#include "BVH/bvh_builder.hpp"

struct GpuMaterial
{
	Int32 albedo;
	Int32 normal;
	Int32 roughness;
	Int32 metalness;
	Int32 emission;
	Float32 indexOfRefraction;
};

struct Camera;

class SRaytraceManager
{
public:
	SRaytraceManager(SRaytraceManager&) = delete;
	static SRaytraceManager& get();

	Void startup();

	Void update(Camera& camera, Float32 deltaTime);

	[[nodiscard]]
	Int32 get_frame_count() const;
	[[nodiscard]]
	glm::vec3 get_background_color() const;
	[[nodiscard]]
	const Texture& get_screen_texture() const;

	Void refresh();
	Void reload_shaders();
	Void shutdown();

	Int32 maxBouncesCount;
	Int32 frameLimit;
private:
	SRaytraceManager() = default;
	~SRaytraceManager() = default;
	static constexpr glm::ivec2 WORKGROUP_SIZE{ 16, 16 };

	Shader rayGeneration, rayTrace, screen;
	BVHBuilder bvh;
	Texture screenTexture, directionTexture;
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
	Int32 frameCount, trianglesCount;
	Bool shouldRefresh;


	Void ray_trace(Camera& camera);
	Void generate_rays(Camera& camera);
	Void resize_opengl_texture(Texture& texture);
};

