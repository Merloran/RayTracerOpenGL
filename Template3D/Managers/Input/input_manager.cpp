#include "input_manager.hpp"

#include "../Display/display_manager.hpp"
#include "Common/input_key.hpp"

#include <GLFW/glfw3.h>

SInputManager& SInputManager::get()
{
	static SInputManager instance;
	return instance;
}

Void SInputManager::startup()
{
	SPDLOG_INFO("Input Manager startup.");
	GLFWwindow* window = &SDisplayManager::get().get_window();
	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, [](GLFWwindow* window, Int32 key, Int32 scancode, Int32 action, Int32 mods) {
		SInputManager& input = *static_cast<SInputManager*>(glfwGetWindowUserPointer(window));
		EInputKey inputKey = s_glfw_key_to_input_key(key);

		if (mods & GLFW_MOD_CONTROL) {
			input.keyState[EInputKey::LEFT_CONTROL] = 1.f;
		}
		else {
			input.keyState[EInputKey::LEFT_CONTROL] = 0.f;
		}

		Float32 value = 0.f;
		switch (action) {
			case GLFW_PRESS:
			{
				value = 1.f;
				break;
			}
			case GLFW_RELEASE:
			{
				value = 0.f;
				break;
			}
			case GLFW_REPEAT:
			default:
				return;
		}
		input.keyState[inputKey] = value;
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, Int32 button, Int32 action, Int32 mods) {
		SInputManager& input = *static_cast<SInputManager*>(glfwGetWindowUserPointer(window));
		const EInputKey inputKey = s_glfw_mouse_button_to_input_key(button);
		input.keyState[inputKey] = action == GLFW_PRESS ? 1.f : 0.f;
	});
	
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, Float64 xpos, Float64 ypos) {
		SInputManager& input = *static_cast<SInputManager*>(glfwGetWindowUserPointer(window));
		input.mousePosition = glm::vec2(xpos, ypos);
	});
}

Void SInputManager::shutdown()
{
	SPDLOG_INFO("Input Manager shutdown.");
}
