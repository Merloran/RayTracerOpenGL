#pragma once
#undef near
#undef far

class Camera
{
public:
	Void initialize(const glm::vec3& position);
	Void move_forward(Float32 dt);
	Void move_right(Float32 dt);
	Void move_up(Float32 dt);
	Void rotate(Float32 xOffset, Float32 yOffset);

	[[nodiscard]]
	const glm::mat4& get_view() const;
	[[nodiscard]]
	const glm::mat4& get_projection(Float32 aspectRatio) const;

	[[nodiscard]]
	const glm::vec3& get_forward() const;
	[[nodiscard]]
	const glm::vec3& get_right() const;
	[[nodiscard]]
	const glm::vec3& get_up() const;
	[[nodiscard]]
	const glm::vec3& get_position() const;
	[[nodiscard]]
	const glm::vec2& get_view_bounds() const;
	[[nodiscard]]
	Float32 get_sensitivity() const;
	[[nodiscard]]
	Float32 get_speed() const;
	[[nodiscard]]
	Float32 get_fov() const;
	[[nodiscard]]
	Bool has_changed() const;

	Void set_position(const glm::vec3& position);
	Void set_view_bounds(const glm::vec2& viewBounds);
	Void set_sensitivity(Float32 sensitivity);
	Void set_speed(Float32 speed);
	Void set_fov(Float32 fov);
	Void set_camera_changed(Bool hasChanged);

	Void catch_input(Float32 deltaTime);

	Void update_camera_vectors();

private:
	glm::vec3 forward, up, right;
	glm::vec3 position;
	glm::vec2 lastPosition, viewBounds;
	Float32   yaw, pitch, fov;
	Float32   speed, sensitivity;

	Bool	  isInactive, hasChanged;
};
