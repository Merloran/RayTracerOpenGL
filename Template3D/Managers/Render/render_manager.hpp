#pragma once
class SRenderManager
{
public:
	SRenderManager(SRenderManager&) = delete;
	static SRenderManager& get();

	void startup();

	void shutdown();

private:
	SRenderManager() = default;
	~SRenderManager() = default;
};

