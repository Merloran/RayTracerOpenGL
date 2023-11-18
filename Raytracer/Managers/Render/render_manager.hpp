#pragma once

class Shader;
struct Model;
class SRenderManager
{
public:
	SRenderManager(SRenderManager&) = delete;
	static SRenderManager& get();

	Void startup();
	Void draw_model(const Model& model, Shader &shader);

	static Void gl_debug(UInt32 source,
	                     UInt32 type,
	                     UInt32 id,
	                     UInt32 severity,
	                     Int32  length,
	                     const Char* message,
	                     const Void* userParam);

	Void shutdown();

private:
	SRenderManager() = default;
	~SRenderManager() = default;
};

