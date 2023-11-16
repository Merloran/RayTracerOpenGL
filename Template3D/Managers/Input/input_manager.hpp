#pragma once

enum class EInputKey;
class SInputManager
{
public:
	SInputManager(SInputManager&) = delete;
	static SInputManager& get();

	Void startup();

	Void shutdown();

private:
	SInputManager() = default;
	~SInputManager() = default;

	glm::vec2 lastMousePosition{};
	glm::vec2 mousePosition{};
	glm::vec2 mouseDelta{};

	std::unordered_map<EInputKey, float> keyState{};
};

