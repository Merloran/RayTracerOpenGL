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
	Void shutdown();

private:
	SRenderManager() = default;
	~SRenderManager() = default;
};

