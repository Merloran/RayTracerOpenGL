#pragma once
class SRenderManager
{
public:
	SRenderManager(SRenderManager&) = delete;
	static SRenderManager& get();

	Void startup();
	Void draw_model(const struct Model& model);
	Void shutdown();

private:
	SRenderManager() = default;
	~SRenderManager() = default;
};

