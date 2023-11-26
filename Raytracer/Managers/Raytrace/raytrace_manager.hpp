#pragma once
#include "../Render/Common/shader.hpp"

struct Camera;

class SRaytraceManager
{
public:
	SRaytraceManager(SRaytraceManager&) = delete;
	static SRaytraceManager& get();

	Void startup();

	Void update(Camera& camera);
	Void create_opengl_texture(UInt32& texture, const glm::ivec2 &size);

	Void shutdown();

private:
	Shader triangle, screen;
	glm::vec3 originPixel, pixelDeltaU, pixelDeltaV;
	UInt32 ssbo[4], screenTexture;
	std::vector<glm::vec4> positions;
	std::vector<UInt32> indexes;

	SRaytraceManager() = default;
	~SRaytraceManager() = default;
};

