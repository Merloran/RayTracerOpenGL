#pragma once

struct GLFWwindow;

class SDisplayManager
{
public:
	SDisplayManager(SDisplayManager&) = delete;
	static SDisplayManager& get();

	Void startup();

	const glm::ivec2& get_frame_buffer_size();
	const glm::ivec2& get_window_size();
	Float32 get_aspect_ratio() const;

	Void poll_events();
	Void make_context_current();
	Void swap_buffers();
	Void close_window();
	Bool should_window_close();
	GLFWwindow& get_window();

	Void shutdown();

private:
	SDisplayManager() = default;
	~SDisplayManager() = default;

	std::string name		  = "BaseWindow";
	GLFWwindow* window		  = nullptr;
	glm::ivec2 windowSize	  = { 800, 600 };
	glm::ivec2 frameBufferSize;
};

