#pragma once
#undef near
#undef far

class Camera
{
public:
	glm::vec3 position{ 0.0f };
	Float32   yaw{ 0.0f }, pitch{ 0.0f };
	Float32   speed{ 400.0f };
	Float32   sensitivity{ 50.0f };
	Float32   near{ 0.1f }, far{ 8000.0f }, fov{ 70.0f };

	Float32   lastX, lastY;
	Bool	  isInactive = true;

	Void initialize(const glm::vec3& position);
	Void move_forward(Float32 dt);
	Void move_right(Float32 dt);
	Void move_up(Float32 dt);
	Void rotate(Float32 xOffset, Float32 yOffset);

	[[nodiscard]]
	const glm::mat4& get_view() const;
	[[nodiscard]]
	const glm::mat4& get_projection(const Float32 aspectRatio) const;

	Void get_input(Float32 deltaTime);

	Void update_camera_vectors();
private:
	glm::vec3 forward{ 0.0f, 0.0f, -1.0f },
			  up{ 0.0f, 1.0f, 0.0f },
			  right{ 1.0, 0.0f, 0.0f };
};
