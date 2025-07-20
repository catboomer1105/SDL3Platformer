#pragma once

#include <optional>
#include <string>

#include <SDL3/SDL_rect.h>

namespace engine::render {

class Sprite final {
private:
	std::string texture_id;
	std::optional<SDL_FRect> source_rect;
	bool is_flipped = false;

public:
	Sprite(const std::string &texture_id, const std::optional<SDL_FRect> &source_rect = std::nullopt, bool is_flipped = false) :
			texture_id(texture_id), source_rect(source_rect), is_flipped(is_flipped) {}

	[[nodiscard]] const std::string &getTextureId() const {
		return texture_id;
	}

	[[nodiscard]] const std::optional<SDL_FRect> &getSourceRect() const {
		return source_rect;
	}

	[[nodiscard]] bool isFlipped() const {
		return is_flipped;
	}

	void setTextureId(const std::string &id) {
		texture_id = id;
	}

	void setSourceRect(const std::optional<SDL_FRect> &rect) {
		source_rect = rect;
	}

	void setFlipped(bool flipped) {
		is_flipped = flipped;
	}
};

} //namespace engine::render