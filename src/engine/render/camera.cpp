#include "camera.h"
#include "../utils/math.h"
#include <spdlog/spdlog.h>

namespace engine::render {

Camera::Camera(const glm::vec2 &viewport_size, const glm::vec2 &position, const std::optional<engine::utils::Rect> limit_bounds) :
		viewport_size(viewport_size), position(position), limit_bounds(limit_bounds) {
	spdlog::trace("Camera 初始化成功，位置: {},{}", position.x, position.y);
}

void Camera::setPosition(const glm::vec2 &newposition) {
	position = newposition;
	clampPosition();
}

void Camera::update(float /* delta_time */) {
	// TODO 自动跟随目标
}

void Camera::move(const glm::vec2 &offset) {
	position += offset;
	clampPosition();
}

void Camera::setLimitBounds(const engine::utils::Rect &bounds) {
	limit_bounds = bounds;
	clampPosition();
}

const glm::vec2 &Camera::getPosition() const {
	return position;
}

void Camera::clampPosition() {
	// 边界检查需要确保相机视图（position 到 position + viewport_size）在 limit_bounds 内
	if (limit_bounds.has_value() && limit_bounds->size.x > 0 && limit_bounds->size.y > 0) {
		// 计算允许的相机位置范围
		glm::vec2 min_cam_pos = limit_bounds->position;
		glm::vec2 max_cam_pos = limit_bounds->position + limit_bounds->size - viewport_size;

		// 确保 max_cam_pos 不小于 min_cam_pos (视口可能比世界还大)
		max_cam_pos.x = std::max(min_cam_pos.x, max_cam_pos.x);
		max_cam_pos.y = std::max(min_cam_pos.y, max_cam_pos.y);

		position = glm::clamp(position, min_cam_pos, max_cam_pos);
	}
	// 如果 limit_bounds 无效则不进行限制
}

glm::vec2 Camera::worldToScreen(const glm::vec2 &world_pos) const {
	// 将世界坐标减去相机左上角位置
	return world_pos - position;
}

glm::vec2 Camera::worldToScreenWithParallax(const glm::vec2 &world_pos, const glm::vec2 &scroll_factor) const {
	// 相机位置应用滚动因子
	return world_pos - position * scroll_factor;
}

glm::vec2 Camera::screenToWorld(const glm::vec2 &screen_pos) const {
	// 将屏幕坐标加上相机左上角位置
	return screen_pos + position;
}

glm::vec2 Camera::getViewportSize() const {
	return viewport_size;
}

std::optional<engine::utils::Rect> Camera::getLimitBounds() const {
	return limit_bounds;
}

} // namespace engine::render