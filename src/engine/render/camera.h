#pragma once

#include <optional>

#include "../utils/math.h"

namespace engine::render {

class Camera final {
private:
	glm::vec2 viewport_size;
	glm::vec2 position;
	std::optional<engine::utils::Rect> limit_bounds;

public:
	Camera(const glm::vec2 &viewport_size, const glm::vec2 &position = glm::vec2(0.0f, 0.0f), const std::optional<engine::utils::Rect> limit_bounds = std::nullopt);

	void update(float delta_time);
	void move(const glm::vec2 &offset);

	glm::vec2 worldToScreen(const glm::vec2 &world_pos) const;
	glm::vec2 worldToScreenWithParallax(const glm::vec2 &world_pos, const glm::vec2 &scroll_factor) const; ///< @brief 世界坐标转屏幕坐标，考虑视差滚动
	glm::vec2 screenToWorld(const glm::vec2 &screen_pos) const;

	void setPosition(const glm::vec2 &position);
	void setLimitBounds(const engine::utils::Rect &bounds);

	const glm::vec2 &getPosition() const;
	std::optional<engine::utils::Rect> getLimitBounds() const;
	glm::vec2 getViewportSize() const;

	Camera(const Camera &) = delete;
	Camera &operator=(const Camera &) = delete;
	Camera(Camera &&) = delete;
	Camera &operator=(Camera &&) = delete;

private:
	void clampPosition();
};

} //namespace engine::render