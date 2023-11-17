#include "camera.hpp"

#include <GLFW/glfw3.h>
#include "../../Display/display_manager.hpp"

Void Camera::initialize(const glm::vec3& position)
{
	this->position = position;
	yaw = -45.0f;
	pitch = 0.0f;

	update_camera_vectors();
}

Void Camera::move_forward(Float32 dt)
{
	position += forward * dt * speed;
}

Void Camera::move_right(Float32 dt)
{
	position += right * dt * speed;
}

Void Camera::move_up(Float32 dt)
{
	position += glm::vec3(0.0f, 1.0f, 0.0f) * dt * speed;
}

Void Camera::rotate(Float32 xOffset, Float32 yOffset)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update_camera_vectors();
}

const glm::mat4& Camera::get_view() const
{
	return glm::lookAt(position, position + forward, up);
}

const glm::mat4& Camera::get_projection(const Float32 aspectRatio) const
{
	return glm::perspective(glm::radians(fov), aspectRatio, near, far);
}

Void Camera::get_input(Float32 deltaTime)
{
	SDisplayManager& displayManager = SDisplayManager::get();

	if (glfwGetKey(&displayManager.get_window(), GLFW_KEY_E) == GLFW_PRESS)
	{
		if (glfwGetInputMode(&displayManager.get_window(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		{
			glfwSetInputMode(&displayManager.get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			isInactive = true;
		} else {
			glfwSetInputMode(&displayManager.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	if (glfwGetInputMode(&displayManager.get_window(), GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
	{
		return;
	}

	Float64 x, y;
	glfwGetCursorPos(&displayManager.get_window(), &x, &y);
	if (isInactive)
	{
		lastX = x;
		lastY = y;
		isInactive = false;
	}

	const Float32 xOffset = x - lastX;
	const Float32 yOffset = lastY - y;
	lastX = x;
	lastY = y;

	rotate(xOffset * deltaTime, yOffset * deltaTime);
	
	// Camera Movement
	if (glfwGetKey(&displayManager.get_window(), GLFW_KEY_W) == GLFW_PRESS)
	{
		move_forward(deltaTime);
	}
	if (glfwGetKey(&displayManager.get_window(), GLFW_KEY_S) == GLFW_PRESS)
	{
		move_forward(-deltaTime);
	}
	if (glfwGetKey(&displayManager.get_window(), GLFW_KEY_A) == GLFW_PRESS)
	{
		move_right(-deltaTime);
	}
	if (glfwGetKey(&displayManager.get_window(), GLFW_KEY_D) == GLFW_PRESS)
	{
		move_right(deltaTime);
	}
	if (glfwGetKey(&displayManager.get_window(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		move_up(deltaTime);
	}
	if (glfwGetKey(&displayManager.get_window(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		move_up(-deltaTime);
	}
	
}

Void Camera::update_camera_vectors()
{
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(forward);

	right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	up = glm::normalize(glm::cross(right, forward));
}
