#pragma once
#undef near
#undef far

class Camera
{
public:
	glm::vec3 position{ 0.0f };
	Float32 yaw{ 0.0f }, pitch{ 0.0f };
	float  speed{ 20.0f };
	Float32 sensitivity{ 20.0f };

	void initialize(const glm::vec3& position);
	void move_forward(Float32 dt);
	void move_right(Float32 dt);
	void move_up(Float32 dt);
	void rotate(Float32 xOffset, Float32 yOffset);

	[[nodiscard]]
	const glm::mat4& get_view() const;
	[[nodiscard]]
	const glm::mat4& get_projection(const Float32 aspectRatio) const;


	void update_camera_vectors();
private:
	glm::vec3 forward{ 0.0f, 0.0f, -1.0f },
		up{ 0.0f, 1.0f, 0.0f },
		right{ 1.0, 0.0f, 0.0f };
	Float32  near{ 0.001f }, far{ 5000.0f }, fov{ 70.0f };
};
