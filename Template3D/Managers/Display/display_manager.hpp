#pragma once

class SDisplayManager
{
public:
	SDisplayManager(SDisplayManager&) = delete;
	static SDisplayManager& get();

	Void startup();

	const glm::ivec2& get_framebuffer_size();
	const glm::ivec2& get_window_size();
	Float32 get_aspect_ratio() const;

	Void update();
	Void close_window();
	Bool should_window_close();
	struct GLFWwindow& get_window() const;

	Void shutdown();

private:
	SDisplayManager() = default;
	~SDisplayManager() = default;

	std::string name		  = "BaseWindow";
	struct GLFWwindow* window = nullptr;
	glm::ivec2 windowSize	  = { 1024, 768 };
	glm::ivec2 framebufferSize;
};

